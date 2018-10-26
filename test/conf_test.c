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

	mushroom_conf_from_args(&conf, 1, (char *[]){ "mushroom" });

	ASSERT_EQ_FMT(6868, conf.gossip_port, "%d");

	PASS();
}

TEST test_conf_from_args(void)
{
	struct mushroom_conf conf = { 0 };
	mushroom_conf_default(&conf);

	mushroom_conf_from_args(&conf, 3, (char *[]){ "mushroom", "-g", "1010" });

	ASSERT_EQ_FMT(1010, conf.gossip_port, "%d");

	PASS();
}

SUITE(conf_suite)
{
	RUN_TEST(test_default_conf);
	RUN_TEST(test_conf_from_args_no_args);
	RUN_TEST(test_conf_from_args);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	RUN_SUITE(conf_suite);

	GREATEST_MAIN_END();
}
