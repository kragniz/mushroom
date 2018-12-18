#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "map.h"

static struct mushroom_map_item MUSHROOM_MAP_DELETED_ITEM = { NULL, NULL };

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
		if (item != NULL && item != &MUSHROOM_MAP_DELETED_ITEM) {
			mushroom_map_item_free(item);
		}
	}
	free(map->items);
	free(map);
}

static int hash(const char *s, int a, int m)
{
	long hash = 0;
	const int len_s = strlen(s);
	for (int i = 0; i < len_s; i++) {
		hash += (long)pow(a, len_s - (i + 1)) * s[i];
		hash = hash % m;
	}
	return (int)hash;
}

static int get_hash(const char *s, int buckets, int attempt)
{
	const int hash_a = hash(s, 163, buckets);
	const int hash_b = hash(s, 173, buckets);
	return (hash_a + (attempt * (hash_b + 1))) % buckets;
}

void mushroom_map_put(struct mushroom_map *map, const char *key, const char *value)
{
	struct mushroom_map_item *item = mushroom_map_item_new(key, value);
	int index = get_hash(item->key, map->size, 0);
	struct mushroom_map_item *conflicting_item = map->items[index];
	int i = 1;
	while (conflicting_item != NULL && conflicting_item != &MUSHROOM_MAP_DELETED_ITEM) {
		index = get_hash(item->key, map->size, i);
		conflicting_item = map->items[index];
		i++;
	}
	map->items[index] = item;
	map->count++;
}

char *mushroom_map_get(struct mushroom_map *map, const char *key)
{
	int index = get_hash(key, map->size, 0);
	struct mushroom_map_item *item = map->items[index];
	int i = 1;
	while (item != NULL) {
		if (item != &MUSHROOM_MAP_DELETED_ITEM) {
			if (strcmp(item->key, key) == 0) {
				return item->value;
			}
		}
		index = get_hash(key, map->size, i);
		item = map->items[index];
		i++;
	}
	return NULL;
}

void mushroom_map_delete(struct mushroom_map *map, const char *key)
{
	int index = get_hash(key, map->size, 0);
	struct mushroom_map_item *item = map->items[index];
	int i = 1;
	while (item != NULL) {
		if (item != &MUSHROOM_MAP_DELETED_ITEM) {
			if (strcmp(item->key, key) == 0) {
				mushroom_map_item_free(item);
				map->items[index] = &MUSHROOM_MAP_DELETED_ITEM;
			}
		}
		index = get_hash(key, map->size, i);
		item = map->items[index];
		i++;
	}
	map->count--;
}
