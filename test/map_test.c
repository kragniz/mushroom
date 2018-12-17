#include "map.h"

#include "greatest.h"

TEST test_map(void)
{
	struct mushroom_map *map = mushroom_map_new();
	ASSERT_EQ_FMT(0, map->count, "%d");

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
