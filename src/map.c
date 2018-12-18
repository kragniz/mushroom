#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "map.h"

struct mushroom_map *mushroom_map_new()
{
	struct mushroom_map *map = malloc(sizeof(*map));

	map->count = 0;
	map->size = 64;
	map->items = calloc(map->size, sizeof(struct mushroom_map_item *));

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

static void mushroom_map_item_free(struct mushroom_map_item *item)
{
	free(item->key);
	free(item->value);
	free(item);
}

void mushroom_map_free(struct mushroom_map *map)
{
	for (size_t i = 0; i < map->size; i++) {
		struct mushroom_map_item *item = map->items[i];
		if (item != NULL) {
			mushroom_map_item_free(item);
		}
	}
	free(map->items);
	free(map);
}
