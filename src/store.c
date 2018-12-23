#include "store.h"
#include "log.h"
#include "map.h"
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
	free(store);
}
