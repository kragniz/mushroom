#pragma once

#include <uv.h>

struct mushroom_gossip_server {
	uv_loop_t *loop;
	struct sockaddr_in *addr;
	uv_udp_t *server;
};

struct mushroom_gossip_server *mushroom_gossip_server_new(uv_loop_t *loop, int port);

void mushroom_gossip_server_start(struct mushroom_gossip_server *server);