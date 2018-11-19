#include <stdio.h>
#include <stdlib.h>

#include <http_parser.h>
#include <uv.h>

#include "api.h"
#include "log.h"

struct mushroom_api *mushroom_api_new(uv_loop_t *loop, const char *addr, int port)
{
	struct mushroom_api *api = calloc(1, sizeof(*api));
	api->addr = calloc(1, sizeof(*api->addr));

	api->loop = loop;

	mushroom_log_debug("created mushroom_api:\t%p", api);

	uv_ip4_addr(addr, port, api->addr);
	mushroom_log_info("api listening on %s:%i", addr, port);

	return api;
}

void mushroom_api_start(struct mushroom_api *api)
{
}