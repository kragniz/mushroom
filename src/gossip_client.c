#include <stdlib.h>

#include <uv.h>

#include "gossip_builder.h"
#include "gossip_client.h"
#include "gossip_json_printer.h"
#include "log.h"
#include "ring.h"

static void on_alloc(uv_handle_t *client, size_t suggested_size, uv_buf_t *buf)
{
	buf->base = malloc(suggested_size);
	buf->len = suggested_size;
}

static void after_write(uv_write_t *req, int status)
{
	assert(status == 0);
	free(req);
}

static void write_gossip_message(struct mushroom_gossip_client *client, uv_stream_t *stream)
{
	uv_buf_t gossip_msg;
	on_alloc(NULL, 256, &gossip_msg);
	memset(gossip_msg.base, 0, gossip_msg.len);

	mushroom_join_request_ref_t join_request =
		mushroom_join_request_create(client->builder, 100);
	mushroom_message_contents_union_ref_t contents =
		mushroom_message_contents_as_join_request(join_request);
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
	mushroom_log_debug("sending gossip message: %s", buf);

	uv_write_t *req = malloc(sizeof(*req));
	if (uv_write(req, stream, &gossip_msg, 1, after_write)) {
		mushroom_log_error("uv_write failed");
	}

	flatcc_builder_reset(client->builder);
}

static void on_connect(uv_connect_t *req, int status)
{
	struct mushroom_gossip_client *client = (struct mushroom_gossip_client *)req->data;

	write_gossip_message(client, req->handle);
}

static void gossip_event_callback(uv_timer_t *handle)
{
	struct mushroom_gossip_client *client = (struct mushroom_gossip_client *)handle->data;

	if (client->ring->node_count <= 0) {
		mushroom_log_debug("no nodes to gossip with");
		return;
	} else {
		mushroom_log_debug("gossiping");
	}

	uv_buf_t gossip_msg;
	on_alloc(NULL, 256, &gossip_msg);
	memset(gossip_msg.base, 0, gossip_msg.len);

	uv_tcp_t *send_socket = malloc(sizeof(*send_socket));
	uv_tcp_init(client->loop, send_socket);

	struct sockaddr_in send_addr;
	uv_ip4_addr(client->ring->nodes[0]->address, client->ring->nodes[0]->gossip_port,
		    &send_addr);

	uv_connect_t *connect = malloc(sizeof(*connect));
	connect->data = client;
	uv_tcp_connect(connect, send_socket, (const struct sockaddr *)&send_addr, on_connect);
}

struct mushroom_gossip_client *mushroom_gossip_client_new(uv_loop_t *loop,
							  struct mushroom_ring *ring)
{
	struct mushroom_gossip_client *client = calloc(1, sizeof(*client));
	client->loop = loop;
	client->ring = ring;

	client->builder = malloc(sizeof(*client->builder));
	flatcc_builder_init(client->builder);

	client->timer = malloc(sizeof(*client->timer));
	uv_timer_init(loop, client->timer);
	client->timer->data = (void *)client;

	mushroom_log_debug("created mushroom_gossip_client: %p", client);

	return client;
}

void mushroom_gossip_client_start(struct mushroom_gossip_client *client)
{
	int err = uv_timer_start(client->timer, gossip_event_callback, 500, 5000);
	if (err < 0) {
		mushroom_log_fatal("timer_start: %s", uv_strerror(err));
	}
}
