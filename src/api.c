#include <stdio.h>
#include <stdlib.h>

#include <http_parser.h>
#include <uv.h>

#include "api.h"
#include "log.h"

#define RESPONSE                                                                                   \
	"HTTP/1.1 200 OK\r\n"                                                                      \
	"Content-Type: application/json\r\n"                                                       \
	"Content-Length: 22\r\n"                                                                   \
	"\r\n"                                                                                     \
	"and you've been gnomed\n"

static http_parser_settings parser_settings;

struct client {
	uv_tcp_t handle;
	http_parser parser;
};

static void on_alloc(uv_handle_t *client, size_t suggested_size, uv_buf_t *buf)
{
	buf->base = malloc(suggested_size);
	buf->len = suggested_size;
}

void on_close(uv_handle_t *handle)
{
	struct client *client = (struct client *)handle->data;
	free(client);
}

void on_shutdown(uv_shutdown_t *shutdown_req, int status)
{
	uv_close((uv_handle_t *)shutdown_req->handle, on_close);
	free(shutdown_req);
}

void on_read(uv_stream_t *handle, ssize_t nread, const uv_buf_t *buf)
{
	int err = 0;
	struct client *client = (struct client *)handle->data;

	if (nread >= 0) {
		size_t parsed =
			http_parser_execute(&client->parser, &parser_settings, buf->base, nread);

		if (parsed < (size_t)nread) {
			mushroom_log_error("parse error");
			uv_close((uv_handle_t *)handle, on_close);
		}
	} else {
		if (nread == UV_EOF) {
			// do nothing
		} else {
			mushroom_log_error("read: %s", uv_strerror(nread));
		}

		uv_shutdown_t *shutdown_req = malloc(sizeof(uv_shutdown_t));
		err = uv_shutdown(shutdown_req, handle, on_shutdown);
		if (err < 0) {
			mushroom_log_error("shutdown: %s", uv_strerror(nread));
		}
	}
	free(buf->base);
}

void on_connection(uv_stream_t *server, int status)
{
	if (status < 0) {
		mushroom_log_error("on_connection: %s", uv_strerror(status));
	}

	struct client *client = calloc(1, sizeof(*client));
	int err = uv_tcp_init(server->loop, &client->handle);
	if (err < 0) {
		mushroom_log_error("on_connection: %s", uv_strerror(status));
	}

	client->handle.data = client;

	err = uv_accept(server, (uv_stream_t *)&client->handle);
	if (err) {
		uv_shutdown_t *shutdown_req = malloc(sizeof(*shutdown_req));
		uv_shutdown(shutdown_req, (uv_stream_t *)&client->handle, on_shutdown);
		if (err < 0) {
			mushroom_log_error("accept: %s", uv_strerror(status));
		}
	}

	http_parser_init(&client->parser, HTTP_REQUEST);
	client->parser.data = client;
	err = uv_read_start((uv_stream_t *)&client->handle, on_alloc, on_read);
	if (err < 0) {
		mushroom_log_error("on_connection: %s", uv_strerror(status));
	}
}

void on_write(uv_write_t *write_req, int status)
{
	uv_close((uv_handle_t *)write_req->handle, on_close);
	free(write_req);
}

int on_headers_complete(http_parser *parser)
{
	struct client *client = (struct client *)parser->data;

	uv_write_t *write_req = malloc(sizeof(uv_write_t));
	uv_buf_t buf = uv_buf_init(RESPONSE, sizeof(RESPONSE));
	int err = uv_write(write_req, (uv_stream_t *)&client->handle, &buf, 1, on_write);
	if (err < 0) {
		mushroom_log_error("write: %s", uv_strerror(err));
	}

	return 1;
}

struct mushroom_api *mushroom_api_new(uv_loop_t *loop, const char *addr, int port)
{
	struct mushroom_api *api = calloc(1, sizeof(*api));
	api->addr = calloc(1, sizeof(*api->addr));

	api->loop = loop;

	api->server = malloc(sizeof(*api->server));
	int err = uv_tcp_init(loop, api->server);
	if (err < 0) {
		mushroom_log_fatal("init: %s", uv_strerror(err));
	}

	err = uv_ip4_addr(addr, port, api->addr);
	if (err < 0) {
		mushroom_log_fatal("init: %s", uv_strerror(err));
	}

	mushroom_log_debug("created mushroom_api:\t%p", api);

	mushroom_log_info("api listening on %s:%i", addr, port);

	return api;
}

void mushroom_api_start(struct mushroom_api *api)
{
	parser_settings.on_headers_complete = on_headers_complete;

	int err = uv_tcp_bind(api->server, (struct sockaddr *)api->addr, 0);
	if (err < 0) {
		mushroom_log_fatal("bind: %s", uv_strerror(err));
	}

	err = uv_listen((uv_stream_t *)api->server, 128, on_connection);
	if (err < 0) {
		mushroom_log_fatal("listen: %s", uv_strerror(err));
	}
}
