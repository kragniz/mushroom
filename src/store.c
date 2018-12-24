#include <stdio.h>
#include <string.h>

#include "log.h"
#include "map.h"
#include "store.h"
#include "workqueue.h"

struct mushroom_store *mushroom_store_new()
{
	struct mushroom_store *store = malloc(sizeof(*store));

	store->map = mushroom_map_new();
	store->replication_queue = mushroom_workqueue_new();

	mushroom_log_debug("created mushroom_store: %p", store);

	return store;
}

void mushroom_store_free(struct mushroom_store *store)
{
	mushroom_map_free(store->map);
	mushroom_workqueue_free(store->replication_queue);
	free(store);
}

void mushroom_store_put(struct mushroom_store *store, struct mushroom_store_instruction_put *put)
{
	size_t key_len = strlen(put->key.name);
	size_t space_len = strlen(put->key.space);
	size_t buf_len = key_len + space_len + 2;
	char key_buf[buf_len];

	snprintf(key_buf, buf_len, "%s/%s", put->key.space, put->key.name);

	mushroom_log_debug("putting %s in the store", key_buf);

	mushroom_map_put(store->map, key_buf, put->value);
}
