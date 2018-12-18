#include "map.h"

#include "greatest.h"

TEST test_map(void)
{
	struct mushroom_map *map = mushroom_map_new();
	ASSERT_EQ_FMT((size_t)0, map->count, "%zu");

	mushroom_map_put(map, "key", "value");

	mushroom_map_free(map);

	PASS();
}

SUITE(map_suite)
{
	RUN_TEST(test_map);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	RUN_SUITE(map_suite);

	GREATEST_MAIN_END();
}
