#include <stdlib.h>

#include <uv.h>

#include "gossip_builder.h"
#include "gossip_client.h"
#include "log.h"
#include "ring.h"

static void gossip_event_callback(uv_timer_t *handle)
{
	mushroom_log_debug("gossiping");
	struct mushroom_gossip_client *client = (struct mushroom_gossip_client *)handle->data;

	if (client->ring->node_count <= 0) {
		mushroom_log_debug("no nodes to gossip with");
	}
}

struct mushroom_gossip_client *mushroom_gossip_client_new(uv_loop_t *loop,
							  struct mushroom_ring *ring)
{
	struct mushroom_gossip_client *client = calloc(1, sizeof(*client));
	client->loop = loop;
	client->ring = ring;

	client->builder = malloc(sizeof(*client->builder));
	flatcc_builder_init(client->builder);

	client->timer = malloc(sizeof(*client->timer));
	uv_timer_init(loop, client->timer);
	client->timer->data = (void *)client;

	mushroom_log_debug("created mushroom_gossip_client: %p", client);

	return client;
}

void mushroom_gossip_client_start(struct mushroom_gossip_client *client)
{
	int err = uv_timer_start(client->timer, gossip_event_callback, 500, 1000);
	if (err < 0) {
		mushroom_log_fatal("timer_start: %s", uv_strerror(err));
	}
}