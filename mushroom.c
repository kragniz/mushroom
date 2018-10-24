#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <uv.h>

#include "gossip_server.h"
#include "log.h"

int main(int argc, char **argv)
{
	mushroom_log_debug("starting mushroom");

	uv_loop_t *uv_loop = uv_default_loop();

	struct mushroom_gossip_server *gossip_server = mushroom_gossip_server_new(uv_loop, 6969);
	mushroom_gossip_server_start(gossip_server);

	if (argc != 1) {
		mushroom_log_debug("%s takes no arguments", argv[0]);
		return 1;
	}

	return uv_run(uv_loop, UV_RUN_DEFAULT);
}
