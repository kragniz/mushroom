#include <stdio.h>
#include <string.h>

#include "ring.h"
#include "node.h"

#include "test.h"

int tests_run = 0;

static char *test_ring()
{
	struct mushroom_ring *ring = mushroom_ring_new();

	mshrm_assert("error, node_count not set to 0", ring->node_count == 0);

	struct mushroom_node *node = mushroom_node_new(0, "127.0.0.1");
	mushroom_ring_add_node(ring, node);

	mushroom_node_free(node);

	struct mushroom_node *node_copy = mushroom_ring_get_node(ring, 0);

	mshrm_assert("error, address not set",
		     strncmp(node_copy->address, "127.0.0.1", 9) == 0);

	mshrm_assert("error, node_count not set to 1", ring->node_count == 1);

	struct mushroom_node *invalid_node = mushroom_ring_get_node(ring, 3);
	mshrm_assert("error, node not set to NULL", invalid_node == NULL);

	return 0;
}

static char *all_tests()
{
	mshrm_run_test(test_ring);
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
