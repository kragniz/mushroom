#pragma once

#include <stdlib.h>

struct mushroom_store {
	struct mushroom_map *map;
};

struct mushroom_store *mushroom_store_new();
void mushroom_store_free(struct mushroom_store *store);
