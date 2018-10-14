#include <stdio.h>
#include <string.h>

#include "node.h"

#include "test.h"

int tests_run = 0;

static char *test_node()
{
	struct mushroom_node *node = mushroom_node_new(0, "127.0.0.1");

	mshrm_assert("error, id not set", node->id == 0);
	mshrm_assert("error, address not set",
		     strncmp(node->address, "127.0.0.1", 9) == 0);
	return 0;
}

static char *all_tests()
{
	mshrm_run_test(test_node);
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
