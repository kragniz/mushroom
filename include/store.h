#pragma once

#include <stdint.h>
#include <stdlib.h>

#include "map.h"
#include "workqueue.h"

struct mushroom_store_key {
	char *space;
	char *name;
	uint64_t revision;
};

struct mushroom_store_instruction_put {
	struct mushroom_store_key key;
	char *value;
};

struct mushroom_store {
	struct mushroom_map *map;
	struct mushroom_workqueue *replication_queue;
};

struct mushroom_store *mushroom_store_new();
void mushroom_store_free(struct mushroom_store *store);

void mushroom_store_put(struct mushroom_store *store, struct mushroom_store_instruction_put *put);
