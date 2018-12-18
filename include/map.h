#pragma once

#include <stdlib.h>

struct mushroom_map_item {
	char *key;
	char *value;
};

struct mushroom_map {
	size_t size;
	size_t count;
	struct mushroom_map_item **items;
};

struct mushroom_map *mushroom_map_new();
void mushroom_map_free(struct mushroom_map *map);
void mushroom_map_put(struct mushroom_map *map, const char *key, const char *value);
char *mushroom_map_get(struct mushroom_map *map, const char *key);
void mushroom_map_delete(struct mushroom_map *map, const char *key);
