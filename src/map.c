#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "map.h"

/* special item to indicate an item used to be stored in its location */
static struct mushroom_map_item MUSHROOM_MAP_DELETED_ITEM = { NULL, NULL };

static bool is_prime(int x)
{
	if ((x % 2) == 0) {
		return false;
	}
	for (int i = 3; i <= floor(sqrt((double)x)); i += 2) {
		if ((x % i) == 0) {
			return 0;
		}
	}
	return true;
}

static int next_prime(int x)
{
	while (!is_prime(x)) {
		x++;
	}
	return x;
}

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

size_t mushroom_map_get_size(struct mushroom_map *map)
{
	return map->size;
}

size_t mushroom_map_get_count(struct mushroom_map *map)
{
	return map->count;
}

static bool mushroom_map_needs_resize(const struct mushroom_map *map)
{
	const int load = (mushroom_map_get_count(map) * 100) / mushroom_map_get_size(map);
	return (load > 70);
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
	const int hash_a = hash(s, 151, buckets);
	const int hash_b = hash(s, 181, buckets);
	return (hash_a + (attempt * (hash_b + 1))) % buckets;
}

void mushroom_map_put(struct mushroom_map *map, const char *key, const char *value)
{
	struct mushroom_map_item *item_to_put = mushroom_map_item_new(key, value);
	int index = get_hash(item_to_put->key, map->size, 0);
	struct mushroom_map_item *current_item = map->items[index];
	int i = 1;

	/* find a different index if the current one is already taken */
	while (current_item != NULL) {
		if (current_item != &MUSHROOM_MAP_DELETED_ITEM) {
			/* if the current item has the same key, replace it */
			if (strcmp(current_item->key, key) == 0) {
				mushroom_map_item_free(current_item);
				map->items[index] = item_to_put;
				return;
			}
		}
		index = get_hash(item_to_put->key, map->size, i);
		current_item = map->items[index];
		i++;
	}
	map->items[index] = item_to_put;
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
