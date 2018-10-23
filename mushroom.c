#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <uv.h>

#include "gossip_server.h"

#define PROJECT_NAME "mushroom"

int main(int argc, char **argv)
{
	uv_loop_t *uv_loop = uv_default_loop();

	mushroom_gossip_server_new(uv_loop, 6969);

	if (argc != 1) {
		printf("%s takes no arguments.\n", argv[0]);
		return 1;
	}
	printf("This is project %s.\n", PROJECT_NAME);

	return uv_run(uv_loop, UV_RUN_DEFAULT);
}
