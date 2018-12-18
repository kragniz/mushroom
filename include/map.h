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