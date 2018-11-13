#include <stdio.h>

#include "node.h"
#include "ring.h"

#include "greatest.h"

TEST test_ring(void)
{
	/* create a new ring */
	struct mushroom_ring *ring = mushroom_ring_new();

	ASSERT_EQ_FMT(ring->node_count, 0, "%i");

	/* add a node to the ring */
	struct mushroom_node *node1 = mushroom_node_new(0, "127.0.0.1", 6969);
	mushroom_ring_add_node(ring, node1);

	mushroom_node_free(node1);

	struct mushroom_node *node_copy = mushroom_ring_get_node(ring, 0);

	ASSERT_STR_EQ(node_copy->address, "127.0.0.1");
	ASSERT_EQ_FMT(ring->node_count, 1, "%i");

	/* try to get a node that doesn't exist */
	struct mushroom_node *invalid_node = mushroom_ring_get_node(ring, 3);
	ASSERT_EQ(invalid_node, NULL);

	/* add a second node */
	struct mushroom_node *node2 = mushroom_node_new(1, "127.0.0.2", 6969);
	mushroom_ring_add_node(ring, node2);

	mushroom_node_free(node2);

	struct mushroom_node *node2_copy = mushroom_ring_get_node(ring, 1);

	ASSERT_STR_EQ(node2_copy->address, "127.0.0.2");
	ASSERT_EQ_FMT(ring->node_count, 2, "%i");

	/* add a whole bunch more nodes */
	for (int i = 2; i < 24; i++) {
		char node_address[24];
		sprintf(node_address, "127.0.0.%i", i);
		struct mushroom_node *n = mushroom_node_new(i, node_address, 6969);
		mushroom_ring_add_node(ring, n);
		mushroom_node_free(n);
	}

	ASSERT_EQ_FMT(ring->node_count, 24, "%i");

	/* get the correct node for a key */
	struct mushroom_node *toot1 = mushroom_ring_node_for_key(ring, "toot1");
	struct mushroom_node *toot2 = mushroom_ring_node_for_key(ring, "toot2");
	struct mushroom_node *toot3 = mushroom_ring_node_for_key(ring, "toot3");

	ASSERT_STR_EQ(toot1->address, "127.0.0.15");
	ASSERT_STR_EQ(toot2->address, "127.0.0.7");
	ASSERT_STR_EQ(toot3->address, "127.0.0.20");

	mushroom_ring_free(ring);

	PASS();
}

TEST test_djb2_hash(void)
{
	uint64_t hello = djb2_hash("hello");
	ASSERT_EQ_FMT(hello, (uint64_t)210714636441, "%lu");

	uint64_t zero = djb2_hash("0");
	ASSERT_EQ_FMT(zero, (uint64_t)177621, "%lu");

	uint64_t empty = djb2_hash("");
	ASSERT_EQ_FMT(empty, (uint64_t)5381, "%lu");

	PASS();
}

SUITE(ring_suite)
{
	RUN_TEST(test_ring);
	RUN_TEST(test_djb2_hash);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	RUN_SUITE(ring_suite);

	GREATEST_MAIN_END();
}
