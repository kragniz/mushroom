#include <stdio.h>

#include "jump.h"

#include "test.h"

int tests_run = 0;

static char *test_jump()
{
	/* test values taken from lithammer/python-jump-consistent-hash */
	int32_t h = jump_consistent_hash(1, 1);
	mshrm_assert("error, jump_consistent_hash != 0", h == 0);

	h = jump_consistent_hash(42, 57);
	mshrm_assert("error, jump_consistent_hash != 43", h == 43);

	h = jump_consistent_hash(0xDEAD10CC, 1);
	mshrm_assert("error, jump_consistent_hash != 0", h == 0);

        h = jump_consistent_hash(0xDEAD10CC, 666);
	mshrm_assert("error, jump_consistent_hash != 361", h == 361);

        h = jump_consistent_hash(256, 1024);
	mshrm_assert("error, jump_consistent_hash != 520", h == 520);
	return 0;
}

static char *all_tests()
{
	mshrm_run_test(test_jump);
	return 0;
}

int main()
{
	char *result = all_tests();
	if (result != 0) {
		printf("%s\n", result);
	} else {
		printf("ALL TESTS PASSED\n");
	}
	printf("Tests run: %d\n", tests_run);

	return result != 0;
}
