#include "store.h"

#include "greatest.h"

TEST test_store(void)
{
	struct mushroom_store *store = mushroom_store_new();

	/* store a value */
	struct mushroom_store_instruction_put put = {
		.key = {
			.space = "default",
			.name = "test",
			.revision = 1,
		},
		.value = "hello there",
	};

	mushroom_store_put(store, &put);

	/* check it exists in the map */
	char *value = mushroom_map_get(store->map, "default/test");
	ASSERT(value != NULL);
	ASSERT_STR_EQ("hello there", value);
	ASSERT_EQ_FMT((size_t)1, mushroom_map_get_count(store->map), "%zu");

	mushroom_store_free(store);

	PASS();
}

SUITE(store_suite)
{
	RUN_TEST(test_store);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	RUN_SUITE(store_suite);

	GREATEST_MAIN_END();
}
