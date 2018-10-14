#include <stdlib.h>

#include "ring.h"

struct mushroom_ring *mushroom_ring_new(void)
{
	struct mushroom_ring *ring = calloc(1, sizeof(struct mushroom_ring));
	ring->node_count = 0;
	ring->nodes = malloc(sizeof(struct mushroom_node) * ring->node_count);

	return ring;
}

void mushroom_ring_free(struct mushroom_ring *ring)
{
	free(ring->nodes);
	free(ring);
}

void mushroom_ring_add_node(struct mushroom_ring *ring,
			    const struct mushroom_node *node)
{
	ring->node_count++;
	ring->nodes = realloc(ring->nodes,
			      sizeof(struct mushroom_node) * ring->node_count);
	ring->nodes[ring->node_count - 1] = *mushroom_node_copy(node);
}

struct mushroom_node *mushroom_ring_get_node(struct mushroom_ring *ring,
					     uint16_t node_index)
{
	if (node_index >= ring->node_count)
		return NULL;

	return &ring->nodes[node_index];
}
