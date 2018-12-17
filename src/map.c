#include <stdlib.h>

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