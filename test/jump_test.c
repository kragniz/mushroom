#include <stdio.h>

#include "jump.h"

#include "test.h"

int tests_run = 0;

static char *test_jump()
{
	int32_t h = jump_consistent_hash(1, 1);
	mshrm_assert("error, jump_consistent_hash != 0", h == 0);

	h = jump_consistent_hash(42, 57);
	mshrm_assert("error, jump_consistent_hash != 43", h == 43);
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
