#pragma once

#include <stdint.h>

struct mushroom_node {
	uint32_t id;
	char *address;
	int gossip_port;
};

struct mushroom_node *mushroom_node_new(uint32_t id, const char *address, int gossip_port);
void mushroom_node_free(struct mushroom_node *node);

struct mushroom_node *mushroom_node_copy(const struct mushroom_node *node);
