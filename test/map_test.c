#include "map.h"

#include "greatest.h"

TEST test_map(void)
{
	struct mushroom_map *map = mushroom_map_new();
	ASSERT_EQ_FMT((size_t)0, mushroom_map_get_count(map), "%zu");

	/* try to get some key that doesn't exist */
	char *value = mushroom_map_get(map, "key-doesn't-exist");
	ASSERT_EQ_FMT(NULL, (void *)value, "%p");

	/* store a value */
	mushroom_map_put(map, "key", "value");
	value = mushroom_map_get(map, "key");
	ASSERT_STR_EQ("value", value);
	ASSERT_EQ_FMT((size_t)1, mushroom_map_get_count(map), "%zu");

	/* update the value to something else */
	mushroom_map_put(map, "key", "value-2");
	value = mushroom_map_get(map, "key");
	ASSERT_STR_EQ("value-2", value);
	ASSERT_EQ_FMT((size_t)1, mushroom_map_get_count(map), "%zu");

	/* delete the item */
	mushroom_map_delete(map, "key");
	value = mushroom_map_get(map, "key");
	ASSERT_EQ_FMT(NULL, (void *)value, "%p");
	ASSERT_EQ_FMT((size_t)0, mushroom_map_get_count(map), "%zu");

	/* write again */
	mushroom_map_put(map, "key", "value-1");
	value = mushroom_map_get(map, "key");
	ASSERT_STR_EQ("value-1", value);
	ASSERT_EQ_FMT((size_t)1, mushroom_map_get_count(map), "%zu");

	mushroom_map_free(map);

	PASS();
}

TEST test_large_map(void)
{
	struct mushroom_map *map = mushroom_map_new();
	ASSERT_EQ_FMT((size_t)0, mushroom_map_get_count(map), "%zu");

	int key_number = 300;

	for (int i = 0; i < key_number; i++) {
		char key[10];
		sprintf(key, "key-%d", i);
		mushroom_map_put(map, key, key);
	}

	char *value = mushroom_map_get(map, "key-9");
	ASSERT(value != NULL);
	ASSERT_STR_EQ("key-9", value);

	for (int i = 0; i < key_number; i++) {
		char key[10];
		sprintf(key, "key-%d", i);
		mushroom_map_put(map, key, key);
		value = mushroom_map_get(map, key);
		ASSERT(value != NULL);
		ASSERT_STR_EQ(key, value);
	}

	value = mushroom_map_get(map, "key-9");
	ASSERT(value != NULL);
	ASSERT_STR_EQ("key-9", value);
	ASSERT_EQ_FMT((size_t)300, mushroom_map_get_count(map), "%zu");

	mushroom_map_free(map);

	PASS();
}

TEST test_weird_string_map(void)
{
	struct mushroom_map *map = mushroom_map_new();
	char key[] = { '\x80', '\0' };
	char value[] = { '\x06', '\0' };
	mushroom_map_put(map, key, value);
	char *v = mushroom_map_get(map, key);
	ASSERT(v != NULL);
	ASSERT_STR_EQ(value, v);
	ASSERT_EQ_FMT((size_t)1, mushroom_map_get_count(map), "%zu");

	mushroom_map_free(map);

	PASS();
}

int for_item_count;

void for_item(struct mushroom_map_item *item)
{
	for_item_count++;
}

TEST test_map_for_each_key(void)
{
	struct mushroom_map *map = mushroom_map_new();

	for (int i = 0; i < 15; i++) {
		char key[10];
		sprintf(key, "key-%d", i);
		mushroom_map_put(map, key, key);
	}

	/* count the number of times the function was called */
	for_item_count = 0;
	mushroom_map_for_each_key(map, &for_item);
	ASSERT_EQ_FMT(15, for_item_count, "%i");

	mushroom_map_free(map);

	PASS();
}

SUITE(map_suite)
{
	RUN_TEST(test_map);
	RUN_TEST(test_large_map);
	RUN_TEST(test_weird_string_map);
	RUN_TEST(test_map_for_each_key);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	RUN_SUITE(map_suite);

	GREATEST_MAIN_END();
}
