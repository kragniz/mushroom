#include <string.h>

#include "node.h"

#include "greatest.h"

TEST test_node(void)
{
	struct mushroom_node *node = mushroom_node_new(0, "127.0.0.1", 6969);

	ASSERT_EQ_FMT(0, node->id, "%d");
	ASSERT_EQ_FMT(6969, node->gossip_port, "%d");
	ASSERT_STR_EQ("127.0.0.1", node->address);

	mushroom_node_free(node);

	PASS();
}

SUITE(node_suite)
{
	RUN_TEST(test_node);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	RUN_SUITE(node_suite);

	GREATEST_MAIN_END();
}
