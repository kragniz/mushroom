#include <stdlib.h>

#include "jump.h"
#include "ring.h"

static size_t _nodes_size(struct mushroom_ring *ring)
{
	return sizeof(struct mushroom_node *) * ring->node_count;
}

struct mushroom_ring *mushroom_ring_new(void)
{
	struct mushroom_ring *ring = calloc(1, sizeof(*ring));
	ring->node_count = 0;
	ring->nodes = malloc(_nodes_size(ring));

	return ring;
}

void mushroom_ring_free(struct mushroom_ring *ring)
{
	for (uint32_t i = 0; i < ring->node_count; i++) {
		mushroom_node_free(ring->nodes[i]);
	}
	free(ring->nodes);
	free(ring);
}

void mushroom_ring_add_node(struct mushroom_ring *ring,
			    const struct mushroom_node *node)
{
	ring->node_count++;
	ring->nodes = realloc(ring->nodes, _nodes_size(ring));
	ring->nodes[ring->node_count - 1] = mushroom_node_copy(node);
}

struct mushroom_node *mushroom_ring_get_node(struct mushroom_ring *ring,
					     uint32_t node_index)
{
	if (node_index >= ring->node_count)
		return NULL;

	return ring->nodes[node_index];
}

/* see http://www.cse.yorku.ca/~oz/hash.html */
uint64_t djb2_hash(char *str)
{
	uint64_t hash = 5381;
	int c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c;

	return hash;
}

struct mushroom_node *mushroom_ring_node_for_key(struct mushroom_ring *ring,
						 char *key_name)
{
	uint64_t key = djb2_hash(key_name);
	uint32_t node_index = jump_consistent_hash(key, ring->node_count);
	return mushroom_ring_get_node(ring, node_index);
}
