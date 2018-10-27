#include "conf.h"

#include "greatest.h"

TEST test_default_conf(void)
{
	struct mushroom_conf conf = { 0 };
	mushroom_conf_default(&conf);

	ASSERT_EQ_FMT(6868, conf.gossip_port, "%d");

	PASS();
}

TEST test_conf_from_args_no_args(void)
{
	struct mushroom_conf conf = { 0 };
	mushroom_conf_default(&conf);

	bool ok = mushroom_conf_from_args(&conf, 1, (char *[]){ "mushroom" });

	ASSERT_EQ(true, ok);
	ASSERT_EQ_FMT(6868, conf.gossip_port, "%d");

	PASS();
}

TEST test_conf_from_args(void)
{
	struct mushroom_conf conf = { 0 };
	mushroom_conf_default(&conf);

	bool ok = mushroom_conf_from_args(&conf, 3, (char *[]){ "mushroom", "-g", "1010" });

	ASSERT_EQ(true, ok);
	ASSERT_EQ_FMT(1010, conf.gossip_port, "%d");

	PASS();
}

TEST test_conf_from_args_long(void)
{
	struct mushroom_conf conf = { 0 };
	mushroom_conf_default(&conf);

	bool ok = mushroom_conf_from_args(&conf, 3, (char *[]){ "mushroom", "--gossip-port", "3030" });

	ASSERT_EQ(true, ok);
	ASSERT_EQ_FMT(3030, conf.gossip_port, "%d");

	PASS();
}

TEST test_conf_from_args_incomplete(void)
{
	struct mushroom_conf conf = { 0 };
	mushroom_conf_default(&conf);

	bool ok = mushroom_conf_from_args(&conf, 2, (char *[]){ "mushroom", "-g" });

	ASSERT_EQ(false, ok);

	PASS();
}

SUITE(conf_suite)
{
	RUN_TEST(test_conf_from_args);
	RUN_TEST(test_conf_from_args_incomplete);
	RUN_TEST(test_conf_from_args_long);
	RUN_TEST(test_conf_from_args_no_args);
	RUN_TEST(test_default_conf);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	RUN_SUITE(conf_suite);

	GREATEST_MAIN_END();
}
