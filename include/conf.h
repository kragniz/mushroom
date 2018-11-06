#pragma once

#include <stdbool.h>

enum mushroom_node_mode {
	MUSHROOM_GROW,
	MUSHROOM_SPORE,
};

struct mushroom_conf {
	enum mushroom_node_mode mode;
	int gossip_port;
	char *gossip_address;
};

void mushroom_conf_default(struct mushroom_conf *conf);
bool mushroom_conf_from_args(struct mushroom_conf *conf, int argc, char *argv[]);
void mushroom_conf_log(struct mushroom_conf *conf);