#pragma once

#include <uv.h>

struct mushroom_gossip_client {
	uv_loop_t *loop;
};

struct mushroom_gossip_client *mushroom_gossip_client_new(uv_loop_t *loop);
void mushroom_gossip_client_start(struct mushroom_gossip_client *client);