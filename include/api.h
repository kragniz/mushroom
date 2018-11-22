#pragma once

#include <http_parser.h>
#include <uv.h>

struct mushroom_api {
	uv_loop_t *loop;
	struct sockaddr_in *addr;
	uv_tcp_t *server;
};

struct mushroom_api *mushroom_api_new(uv_loop_t *loop, const char *addr, int port);

void mushroom_api_start(struct mushroom_api *api);
