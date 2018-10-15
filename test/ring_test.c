#include <stdio.h>
#include <string.h>

#include "ring.h"
#include "node.h"

#include "test.h"

int tests_run = 0;

static char *test_ring()
{
	/* create a new ring */
	struct mushroom_ring *ring = mushroom_ring_new();

	mshrm_assert("error, node_count not set to 0", ring->node_count == 0);

	/* add a node to the ring */
	struct mushroom_node *node1 = mushroom_node_new(0, "127.0.0.1");
	mushroom_ring_add_node(ring, node1);

	mushroom_node_free(node1);

	struct mushroom_node *node_copy = mushroom_ring_get_node(ring, 0);

	mshrm_assert("error, address not set",
		     strncmp(node_copy->address, "127.0.0.1", 9) == 0);

	mshrm_assert("error, node_count not set to 1", ring->node_count == 1);

	/* try to get a node that doesn't exist */
	struct mushroom_node *invalid_node = mushroom_ring_get_node(ring, 3);
	mshrm_assert("error, node not set to NULL", invalid_node == NULL);

	/* add a second node */
	struct mushroom_node *node2 = mushroom_node_new(1, "127.0.0.2");
	mushroom_ring_add_node(ring, node2);

	mushroom_node_free(node2);

	struct mushroom_node *node2_copy = mushroom_ring_get_node(ring, 1);

	mshrm_assert("error, address not set",
		     strncmp(node2_copy->address, "127.0.0.2", 9) == 0);

	mshrm_assert("error, node_count not set to 2", ring->node_count == 2);

	/* add a whole bunch more nodes */
	for (int i = 2; i < 24; i++) {
		char node_address[24];
		sprintf(node_address, "127.0.0.%i", i);
		struct mushroom_node *n = mushroom_node_new(i, node_address);
		mushroom_ring_add_node(ring, n);
		mushroom_node_free(n);
	}

	mshrm_assert("error, node_count not set to 24", ring->node_count == 24);

	/* get the correct node for a key */
	struct mushroom_node *toot1 = mushroom_ring_node_for_key(ring, "toot1");
	struct mushroom_node *toot2 = mushroom_ring_node_for_key(ring, "toot2");
	struct mushroom_node *toot3 = mushroom_ring_node_for_key(ring, "toot3");

	mshrm_assert("error, toot1 assigned to wrong node",
		     strncmp(toot1->address, "127.0.0.15", 9) == 0);

	mshrm_assert("error, toot2 assigned to wrong node",
		     strncmp(toot2->address, "127.0.0.7", 9) == 0);

	mshrm_assert("error, toot3 assigned to wrong node",
		     strncmp(toot3->address, "127.0.0.20", 9) == 0);

	mushroom_ring_free(ring);

	return 0;
}

static char *test_djb2_hash()
{
	uint64_t hello = djb2_hash("hello");
	mshrm_assert("error, hello != 210714636441", hello == 210714636441);

	uint64_t zero = djb2_hash("0");
	mshrm_assert("error, zero != 177621", zero == 177621);

	uint64_t empty = djb2_hash("");
	mshrm_assert("error, empty != 0", empty == 5381);

	return 0;
}

static char *all_tests()
{
	mshrm_run_test(test_ring);
	mshrm_run_test(test_djb2_hash);
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
