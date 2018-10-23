#pragma once

#include "node.h"

struct mushroom_ring {
	uint32_t node_count;
	struct mushroom_node **nodes;
};

struct mushroom_ring *mushroom_ring_new(void);
void mushroom_ring_free(struct mushroom_ring *ring);

void mushroom_ring_add_node(struct mushroom_ring *ring, const struct mushroom_node *node);

struct mushroom_node *mushroom_ring_get_node(struct mushroom_ring *ring, uint32_t node_index);

struct mushroom_node *mushroom_ring_node_for_key(struct mushroom_ring *ring, char *key_name);

uint64_t djb2_hash(char *str);
