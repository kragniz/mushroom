#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "map.h"

struct mushroom_map *mushroom_map_new()
{
	struct mushroom_map *map = malloc(sizeof(*map));

	map->count = 0;
	map->size = 64;

	mushroom_log_debug("created mushroom_map: %p", map);

	return map;
}

static struct mushroom_map_item *mushroom_map_item_new(const char *key, const char *value)
{
	struct mushroom_map_item *item = malloc(sizeof(*item));

	item->key = strdup(key);
	item->value = strdup(value);

	return item;
}