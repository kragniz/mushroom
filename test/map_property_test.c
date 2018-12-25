#include <stdbool.h>
#include <string.h>

#include "greatest.h"
#include "theft.h"

#include "map.h"

static enum theft_trial_res property_valid_keys(struct theft *t, void *arg1, void *arg2)
{
	(void)t;
	char *key = arg1;
	char *value = arg2;

	struct mushroom_map *map = mushroom_map_new();

	mushroom_map_put(map, key, value);
	char *returned_value = mushroom_map_get(map, key);
	int success = strcmp(returned_value, value);

	mushroom_map_free(map);

	return success ? THEFT_TRIAL_FAIL : THEFT_TRIAL_PASS;
}

TEST test_map_valid_keys(void)
{
	theft_seed seed = theft_seed_of_time();

	struct theft_run_config cfg = {
        .name = __func__,
        .prop2 = property_valid_keys,
        .type_info = {
            theft_get_builtin_type_info(THEFT_BUILTIN_char_ARRAY),
            theft_get_builtin_type_info(THEFT_BUILTIN_char_ARRAY),
        },
        .bloom_bits = 20,
        .seed = seed,
        .trials = 100,
		.fork = {
			.enable = true,
			.timeout = 20000,
		},
    };

	ASSERT_EQ(THEFT_RUN_PASS, theft_run(&cfg));

	PASS();
}

SUITE(map_suite)
{
	RUN_TEST(test_map_valid_keys);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	RUN_SUITE(map_suite);

	GREATEST_MAIN_END();
}
