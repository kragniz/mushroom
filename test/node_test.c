#include <string.h>

#include "node.h"

#include "greatest.h"

TEST test_node(void)
{
	struct mushroom_node *node = mushroom_node_new(0, "127.0.0.1");

	ASSERT_EQ_FMT(node->id, 0, "%d");
	ASSERT_STR_EQ(node->address, "127.0.0.1");

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
