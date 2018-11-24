#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <uv.h>

#include "api.h"
#include "conf.h"
#include "gossip_client.h"
#include "gossip_server.h"
#include "log.h"
#include "ring.h"
#include "version.h"

int main(int argc, char **argv)
{
	struct mushroom_conf conf = { 0 };
	mushroom_conf_default(&conf);
	bool ok = mushroom_conf_from_args(&conf, argc, argv);
	if (!ok)
		exit(1);

	mushroom_log_info("🍄 %s", MUSHROOM_BUILD_ID);
	mushroom_conf_log(&conf);

	uv_loop_t *uv_loop = uv_default_loop();

	struct mushroom_gossip_server *gossip_server =
		mushroom_gossip_server_new(uv_loop, conf.gossip_address, conf.gossip_port);
	mushroom_gossip_server_start(gossip_server);

	struct mushroom_ring *ring = mushroom_ring_new();
	if (conf.mode == MUSHROOM_GROW && conf.initial_node_address != NULL)
		mushroom_ring_add_node(ring, mushroom_node_new(0, conf.initial_node_address,
							       conf.initial_node_port));

	struct mushroom_gossip_client *gossip_client = mushroom_gossip_client_new(uv_loop, ring);
	mushroom_gossip_client_start(gossip_client);

	struct mushroom_api *api = mushroom_api_new(uv_loop, conf.api_address, conf.api_port);
	mushroom_api_start(api);

	return uv_run(uv_loop, UV_RUN_DEFAULT);
}
