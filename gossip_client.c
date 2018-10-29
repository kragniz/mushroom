#include <stdlib.h>

#include <uv.h>

#include "gossip_client.h"
#include "log.h"
#include "ring.h"

static void gossip_event_callback(uv_timer_t *handle)
{
	mushroom_log_debug("gossiping");
}

struct mushroom_gossip_client *mushroom_gossip_client_new(uv_loop_t *loop,
							  struct mushroom_ring *ring)
{
	struct mushroom_gossip_client *client = calloc(1, sizeof(*client));
	client->timer = calloc(1, sizeof(*client->timer));
	client->loop = loop;
	client->ring = ring;

	uv_timer_init(loop, client->timer);

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