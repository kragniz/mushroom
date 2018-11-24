#include "conf.h"

#include "greatest.h"

TEST test_default_conf(void)
{
	struct mushroom_conf conf = { 0 };
	mushroom_conf_default(&conf);

	ASSERT_EQ_FMT(6868, conf.gossip_port, "%d");
	ASSERT_STR_EQ("127.0.0.1", conf.gossip_address);

	ASSERT_EQ_FMT(6969, conf.api_port, "%d");
	ASSERT_STR_EQ("127.0.0.1", conf.api_address);

	ASSERT_EQ(MUSHROOM_GROW, conf.mode);

	PASS();
}

TEST test_conf_from_args_no_args(void)
{
	struct mushroom_conf conf = { 0 };
	mushroom_conf_default(&conf);

	bool ok = mushroom_conf_from_args(&conf, 1, (char *[]){ "mushroom" });

	ASSERT_EQ(true, ok);
	ASSERT_EQ_FMT(6868, conf.gossip_port, "%d");
	ASSERT_STR_EQ("127.0.0.1", conf.gossip_address);

	PASS();
}

TEST test_conf_from_args(void)
{
	struct mushroom_conf conf = { 0 };
	mushroom_conf_default(&conf);

	bool ok = mushroom_conf_from_args(&conf, 3,
					  (char *[]){ "mushroom", "--gossip-port", "1010" });

	ASSERT_EQ(true, ok);
	ASSERT_EQ_FMT(1010, conf.gossip_port, "%d");
	ASSERT_STR_EQ("127.0.0.1", conf.gossip_address);

	PASS();
}

TEST test_conf_from_args_addr(void)
{
	struct mushroom_conf conf = { 0 };
	mushroom_conf_default(&conf);

	bool ok = mushroom_conf_from_args(&conf, 5,
					  (char *[]){ "mushroom", "--gossip-address", "0.0.0.0",
						      "--api-address", "10.0.0.1" });

	ASSERT_EQ(true, ok);
	ASSERT_STR_EQ("0.0.0.0", conf.gossip_address);

	ASSERT_EQ_FMT(6969, conf.api_port, "%d");
	ASSERT_STR_EQ("10.0.0.1", conf.api_address);

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

TEST test_conf_mode(void)
{
	struct mushroom_conf conf = { 0 };
	mushroom_conf_default(&conf);

	bool ok = mushroom_conf_from_args(&conf, 2, (char *[]){ "mushroom", "spore" });

	ASSERT_EQ(true, ok);
	ASSERT_EQ(MUSHROOM_SPORE, conf.mode);

	PASS();
}

TEST test_conf_mode_bad(void)
{
	struct mushroom_conf conf = { 0 };
	mushroom_conf_default(&conf);

	bool ok = mushroom_conf_from_args(&conf, 2, (char *[]){ "mushroom", "not-a-real-mode" });

	ASSERT_EQ(false, ok);
	ASSERT_EQ(MUSHROOM_GROW, conf.mode);

	PASS();
}

SUITE(conf_suite)
{
	RUN_TEST(test_conf_from_args);
	RUN_TEST(test_conf_from_args_incomplete);
	RUN_TEST(test_conf_from_args_no_args);
	RUN_TEST(test_default_conf);
	RUN_TEST(test_conf_from_args_addr);
	RUN_TEST(test_conf_mode);
	RUN_TEST(test_conf_mode_bad);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	RUN_SUITE(conf_suite);

	GREATEST_MAIN_END();
}
