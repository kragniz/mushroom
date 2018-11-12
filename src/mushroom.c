#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <uv.h>

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

	struct mushroom_gossip_client *gossip_client = mushroom_gossip_client_new(uv_loop, ring);
	mushroom_gossip_client_start(gossip_client);

	return uv_run(uv_loop, UV_RUN_DEFAULT);
}