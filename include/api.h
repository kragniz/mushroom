#pragma once

#include <http_parser.h>
#include <uv.h>

#include "api_response_builder.h"
#include "store.h"

struct mushroom_api {
	uv_loop_t *loop;
	struct sockaddr_in *addr;
	uv_tcp_t *server;

	flatcc_builder_t *builder;

	struct mushroom_store *store;
};

struct mushroom_api *
mushroom_api_new(uv_loop_t *loop, struct mushroom_store *store, const char *addr, int port);

void mushroom_api_start(struct mushroom_api *api);
