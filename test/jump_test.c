#include "jump.h"

#include "greatest.h"

TEST test_jump(void)
{
	/* test values taken from lithammer/python-jump-consistent-hash */
	int32_t h = jump_consistent_hash(1, 1);

	ASSERT_EQ_FMT(h, 0, "%d");

	h = jump_consistent_hash(42, 57);
	ASSERT_EQ_FMT(h, 43, "%d");

	h = jump_consistent_hash(0xDEAD10CC, 1);
	ASSERT_EQ_FMT(h, 1, "%d");

	h = jump_consistent_hash(0xDEAD10CC, 666);
	ASSERT_EQ_FMT(h, 361, "%d");

	h = jump_consistent_hash(256, 1024);
	ASSERT_EQ_FMT(h, 520, "%d");

	PASS();
}

SUITE(jump_suite)
{
	RUN_TEST(test_jump);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	RUN_SUITE(jump_suite);

	GREATEST_MAIN_END();
}
