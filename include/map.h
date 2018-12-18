#pragma once

struct mushroom_map_item {
	char *key;
	char *value;
};

struct mushroom_map {
	int size;
	int count;
	struct mushroom_map_item **items;
};

struct mushroom_map *mushroom_map_new();