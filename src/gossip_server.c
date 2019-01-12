#include <stdlib.h>

#include <uv.h>

#include "gossip_builder.h"
#include "gossip_json_printer.h"
#include "gossip_reader.h"
#include "gossip_server.h"
#include "log.h"

#define DEFAULT_BACKLOG 128

struct client {
	uv_tcp_t handle;
	uv_buf_t buf;
	flatcc_builder_t *builder;
};

static void handle_gossip_message(mushroom_gossip_message_table_t *msg)
{
	uint32_t from = mushroom_gossip_message_from(*msg);
	mushroom_log_debug("gossip message was from %d", from);
	if (mushroom_gossip_message_contents_type(*msg) == mushroom_message_contents_join_request)
		mushroom_log_debug("gossip message was of type join");
}

static void on_alloc(uv_handle_t *client, size_t suggested_size, uv_buf_t *buf)
{
	buf->base = malloc(suggested_size);
	buf->len = suggested_size;
}

static void on_close(uv_handle_t *handle)
{
	struct client *client = (struct client *)handle->data;
	free(client->buf.base);
	free(client);
}

static void after_write(uv_write_t *req, int status)
{
	assert(status == 0);
	uv_close((uv_handle_t *)req->handle, on_close);
	free(req);
}

static void write_gossip_message(struct client *client, uv_stream_t *stream)
{
	uv_buf_t gossip_msg;
	on_alloc(NULL, 256, &gossip_msg);
	memset(gossip_msg.base, 0, gossip_msg.len);

	mushroom_join_response_ref_t join_response = mushroom_join_response_create(client->builder);
	mushroom_message_contents_union_ref_t contents =
		mushroom_message_contents_as_join_response(join_response);
	mushroom_gossip_message_create_as_root(client->builder, 7070, contents);

	size_t size = flatcc_builder_get_buffer_size(client->builder);
	if (size >= 256)
		mushroom_log_error("flatcc buffer too big: %d", size);
	flatcc_builder_copy_buffer(client->builder, gossip_msg.base, size);

	char buf[1024];
	flatcc_json_printer_t *ctx = malloc(sizeof(*ctx));
	flatcc_json_printer_init_buffer(ctx, buf, 1024);
	gossip_print_json(ctx, gossip_msg.base, size);
	flatcc_json_printer_flush(ctx);
	if (flatcc_json_printer_get_error(ctx)) {
		mushroom_log_error("could not print json");
	}
	free(ctx);
	mushroom_log_debug("sending gossip response: %s", buf);

	uv_write_t *req = malloc(sizeof(*req));
	if (uv_write(req, stream, &gossip_msg, 1, after_write)) {
		mushroom_log_error("uv_write failed");
	}

	flatcc_builder_reset(client->builder);
}

static void on_write(uv_write_t *write_req, int status)
{
	struct client *client = (struct client *)write_req->data;
	write_gossip_message(client, write_req->handle);
}

static void on_shutdown(uv_shutdown_t *shutdown_req, int status)
{
	uv_close((uv_handle_t *)shutdown_req->handle, on_close);
	free(shutdown_req);
}

static void on_read(uv_stream_t *handle, ssize_t nread, const uv_buf_t *buf)
{
	struct client *client = (struct client *)handle->data;

	if (nread > 0) {
		mushroom_gossip_message_table_t msg = mushroom_gossip_message_as_root(buf->base);
		if (msg == NULL) {
			mushroom_log_error("gossip message couldn't be parsed");
		} else {
			handle_gossip_message(&msg);
		}

		uv_write_t *write_req = malloc(sizeof(uv_write_t));
		client->buf = uv_buf_init(buf->base, nread);
		write_req->data = client;
		int err = uv_write(write_req, (uv_stream_t *)&client->handle, &client->buf, 1,
				   on_write);
		if (err < 0) {
			mushroom_log_error("write: %s", uv_strerror(err));
		}
		return;
	} else {
		if (nread != UV_EOF)
			fprintf(stderr, "Read error %s\n", uv_err_name(nread));
		uv_close((uv_handle_t *)client, NULL);
	}

	free(buf->base);
}

void on_connection(uv_stream_t *server, int status)
{
	flatcc_builder_t *builder = (flatcc_builder_t *)server->data;

	mushroom_log_debug("on_connection: client connected");
	struct client *client = calloc(1, sizeof(*client));
	int err = uv_tcp_init(server->loop, &client->handle);
	if (err < 0) {
		mushroom_log_error("on_connection: %s", uv_strerror(status));
	}

	client->handle.data = client;
	client->builder = builder;

	err = uv_accept(server, (uv_stream_t *)&client->handle);
	if (err) {
		uv_shutdown_t *shutdown_req = malloc(sizeof(*shutdown_req));
		uv_shutdown(shutdown_req, (uv_stream_t *)&client->handle, on_shutdown);
		if (err < 0) {
			mushroom_log_error("accept: %s", uv_strerror(status));
		}
	}

	err = uv_read_start((uv_stream_t *)&client->handle, on_alloc, on_read);
	if (err < 0) {
		mushroom_log_error("on_connection: %s", uv_strerror(status));
	}
}

struct mushroom_gossip_server *
mushroom_gossip_server_new(uv_loop_t *loop, const char *addr, int port)
{
	struct mushroom_gossip_server *server = calloc(1, sizeof(*server));
	server->addr = calloc(1, sizeof(*server->addr));
	server->server = calloc(1, sizeof(*server->server));

	server->builder = malloc(sizeof(*server->builder));
	flatcc_builder_init(server->builder);
	server->server->data = server->builder;

	server->loop = loop;

	int err = uv_tcp_init(server->loop, server->server);
	if (err < 0) {
		mushroom_log_fatal("init: %s", uv_strerror(err));
	}

	mushroom_log_debug("created mushroom_gossip_server: %p", server);

	uv_ip4_addr(addr, port, server->addr);
	mushroom_log_info("gossip server listening on %s:%i", addr, port);

	return server;
}

void mushroom_gossip_server_start(struct mushroom_gossip_server *server)
{
	int err = uv_tcp_bind(server->server, (const struct sockaddr *)server->addr, 0);
	if (err < 0) {
		mushroom_log_fatal("bind: %s", uv_strerror(err));
	}

	err = uv_listen((uv_stream_t *)server->server, DEFAULT_BACKLOG, on_connection);
	if (err < 0) {
		mushroom_log_fatal("start: %s", uv_strerror(err));
	}
}
