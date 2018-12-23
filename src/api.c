#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "flatcc/flatcc_verifier.h"
#include <http_parser.h>
#include <uv.h>

#include "api.h"
#include "api_request_builder.h"
#include "api_request_json_parser.h"
#include "api_request_reader.h"
#include "api_request_verifier.h"
#include "api_response_builder.h"
#include "api_response_json_printer.h"
#include "api_response_reader.h"
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
	char *url;
	char *body;
	size_t body_len;
};

static void on_alloc(uv_handle_t *client, size_t suggested_size, uv_buf_t *buf)
{
	buf->base = malloc(suggested_size);
	buf->len = suggested_size;
}

static void on_close(uv_handle_t *handle)
{
	struct client *client = (struct client *)handle->data;
	free(client->url);
	free(client->body);
	free(client);
}

static void on_shutdown(uv_shutdown_t *shutdown_req, int status)
{
	uv_close((uv_handle_t *)shutdown_req->handle, on_close);
	free(shutdown_req);
}

static void on_read(uv_stream_t *handle, ssize_t nread, const uv_buf_t *buf)
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

static void on_connection(uv_stream_t *server, int status)
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

static void on_write(uv_write_t *write_req, int status)
{
	uv_close((uv_handle_t *)write_req->handle, on_close);
	free(write_req);
}

static int on_body(http_parser *parser, const char *buf, size_t len)
{
	struct client *client = (struct client *)parser->data;

	client->body = malloc(len + 1);
	strncpy(client->body, buf, len);
	client->body[len] = '\0';
	client->body_len = len;

	mushroom_log_debug("body: %s", client->body);

	return 0;
}

static int on_headers_complete(http_parser *parser)
{
	struct client *client = (struct client *)parser->data;

	return 0;
}

static int on_url(http_parser *parser, const char *buf, size_t len)
{
	struct client *client = (struct client *)parser->data;

	client->url = malloc(len + 1);
	strncpy(client->url, buf, len);
	client->url[len] = '\0';

	mushroom_log_debug("requested: %s", client->url);

	return 0;
}

static int on_message_complete(http_parser *parser)
{
	struct client *client = (struct client *)parser->data;
	flatcc_builder_t builder;
	flatcc_builder_init(&builder);

	/* parse the incoming request */
	flatcc_json_parser_t parse_ctx;
	bool error_message = false;

	api_request_parse_json(&builder, &parse_ctx, client->body, client->body_len, 0);
	int err = flatcc_json_parser_get_error(&parse_ctx);
	if (err) {
		mushroom_log_error("could not parse json: %s",
				   flatcc_json_parser_error_string(err));
		error_message = true;
	} else {
		size_t out_size = 0;
		void *flatbuffer = flatcc_builder_finalize_aligned_buffer(&builder, &out_size);
		int ret = mushroom_api_message_verify_as_root(flatbuffer, out_size);
		if (ret) {
			mushroom_log_error("could not verify mushroom_api_message: %s",
					   flatcc_verify_error_string(ret));
		}
	}

	/* reinitilise builder */
	flatcc_builder_reset(&builder);

	/* build a response message */
	flatbuffers_string_ref_t value;
	if (error_message) {
		value = flatbuffers_string_create_str(&builder,
						      flatcc_json_parser_error_string(err));
	} else {
		value = flatbuffers_string_create_str(
			&builder, client->body ? client->body : "empty body, POST some data");
	}
	mushroom_api_response_str_ref_t str = mushroom_api_response_str_create(&builder, value);

	mushroom_api_response_value_contents_union_ref_t value_contents;
	if (error_message) {
		value_contents = mushroom_api_response_value_contents_as_error(str);
	} else {
		value_contents = mushroom_api_response_value_contents_as_str(str);
	}

	flatbuffers_buffer_ref_t message =
		mushroom_api_response_message_create_as_root(&builder, value_contents);

	size_t size = flatcc_builder_get_buffer_size(&builder);
	char *fbuf = malloc(size + 1);
	flatcc_builder_copy_buffer(&builder, fbuf, size);

	char jbuf[1024];
	flatcc_json_printer_t *ctx = malloc(sizeof(*ctx));
	flatcc_json_printer_init_buffer(ctx, jbuf, 1024);
	int json_len = api_response_print_json(ctx, fbuf, size);
	flatcc_json_printer_flush(ctx);
	if (flatcc_json_printer_get_error(ctx)) {
		mushroom_log_error("could not print json");
	}
	free(ctx);
	free(fbuf);
	mushroom_log_debug("response: %s", jbuf);

	/*send the response message back */
	uv_write_t *write_req = malloc(sizeof(uv_write_t));
	uv_buf_t buf = uv_buf_init(jbuf, json_len);
	err = uv_write(write_req, (uv_stream_t *)&client->handle, &buf, 1, on_write);
	if (err < 0) {
		mushroom_log_error("write: %s", uv_strerror(err));
	}

	return 0;
}

struct mushroom_api *mushroom_api_new(uv_loop_t *loop, const char *addr, int port)
{
	struct mushroom_api *api = calloc(1, sizeof(*api));
	api->addr = calloc(1, sizeof(*api->addr));

	api->loop = loop;

	api->builder = malloc(sizeof(*api->builder));
	flatcc_builder_init(api->builder);

	api->server = malloc(sizeof(*api->server));
	int err = uv_tcp_init(loop, api->server);
	if (err < 0) {
		mushroom_log_fatal("init: %s", uv_strerror(err));
	}

	err = uv_ip4_addr(addr, port, api->addr);
	if (err < 0) {
		mushroom_log_fatal("init: %s", uv_strerror(err));
	}

	parser_settings.on_url = on_url;
	parser_settings.on_headers_complete = on_headers_complete;
	parser_settings.on_body = on_body;
	parser_settings.on_message_complete = on_message_complete;

	mushroom_log_debug("created mushroom_api:\t%p", api);

	mushroom_log_info("api listening on %s:%i", addr, port);

	return api;
}

void mushroom_api_start(struct mushroom_api *api)
{
	int err = uv_tcp_bind(api->server, (struct sockaddr *)api->addr, 0);
	if (err < 0) {
		mushroom_log_fatal("bind: %s", uv_strerror(err));
	}

	err = uv_listen((uv_stream_t *)api->server, 128, on_connection);
	if (err < 0) {
		mushroom_log_fatal("listen: %s", uv_strerror(err));
	}
}
