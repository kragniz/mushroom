#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <uv.h>

#include "conf.h"
#include "gossip_server.h"
#include "log.h"

int main(int argc, char **argv)
{
	struct mushroom_conf conf = { 0 };
	mushroom_conf_default(&conf);
	bool ok = mushroom_conf_from_args(&conf, argc, argv);
	if (!ok)
		exit(1);

	mushroom_log_debug("starting mushroom");
	mushroom_conf_log(&conf);

	uv_loop_t *uv_loop = uv_default_loop();

	struct mushroom_gossip_server *gossip_server =
		mushroom_gossip_server_new(uv_loop, conf.gossip_port);
	mushroom_gossip_server_start(gossip_server);

	return uv_run(uv_loop, UV_RUN_DEFAULT);
}
