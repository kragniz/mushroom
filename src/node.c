#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "node.h"

struct mushroom_node *mushroom_node_new(uint32_t id, const char *address)
{
	struct mushroom_node *node = calloc(1, sizeof(struct mushroom_node));
	node->id = id;
	node->address = strdup(address);

	mushroom_log_debug("created mushroom_node: %p", node);

	return node;
}

void mushroom_node_free(struct mushroom_node *node)
{
	free(node->address);
	free(node);
}

struct mushroom_node *mushroom_node_copy(const struct mushroom_node *node)
{
	return mushroom_node_new(node->id, node->address);
}
