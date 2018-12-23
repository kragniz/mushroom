#pragma once

#include <stdlib.h>

#include "map.h"
#include "workqueue.h"

struct mushroom_store {
	struct mushroom_map *map;
	struct mushroom_workqueue *replication_queue;
};

struct mushroom_store *mushroom_store_new();
void mushroom_store_free(struct mushroom_store *store);
