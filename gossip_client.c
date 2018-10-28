#include <stdlib.h>

#include <uv.h>

#include "gossip_client.h"
#include "log.h"

struct mushroom_gossip_client *mushroom_gossip_client_new(uv_loop_t *loop)
{
	struct mushroom_gossip_client *client = calloc(1, sizeof(*client));
	client->loop = loop;

	mushroom_log_debug("created mushroom_gossip_client: %p", client);

	return client;
}

void mushroom_gossip_client_start(struct mushroom_gossip_client *client)
{

}