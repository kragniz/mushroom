#pragma once

#include <stdbool.h>

#include "log.h"

enum mushroom_node_mode {
	MUSHROOM_GROW,
	MUSHROOM_SPORE,
};

struct mushroom_conf {
	enum mushroom_log_level log_level;

	enum mushroom_node_mode mode;
	int gossip_port;
	char *gossip_address;

	int initial_node_port;
	char *initial_node_address;

	int api_port;
	char *api_address;
};

void mushroom_conf_default(struct mushroom_conf *conf);
bool mushroom_conf_from_args(struct mushroom_conf *conf, int argc, char *argv[]);
void mushroom_conf_log(struct mushroom_conf *conf);
