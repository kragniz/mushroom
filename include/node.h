#pragma once

#include <stdint.h>

struct mushroom_node {
	uint16_t id;
	char *address;
};

struct mushroom_node *mushroom_node_new(uint16_t id, const char *address);
void mushroom_node_free(struct mushroom_node *node);
