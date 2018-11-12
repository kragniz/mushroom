#pragma once

#include <uv.h>

#include "gossip_builder.h"
#include "ring.h"

struct mushroom_gossip_client {
	uv_loop_t *loop;
	uv_timer_t *timer;
	struct mushroom_ring *ring;

	flatcc_builder_t *builder;
};

struct mushroom_gossip_client *mushroom_gossip_client_new(uv_loop_t *loop,
							  struct mushroom_ring *ring);
void mushroom_gossip_client_start(struct mushroom_gossip_client *client);