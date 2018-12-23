#pragma once

#include <stdbool.h>

#include "log.h"

enum mushroom_node_mode {
	MUSHROOM_GROW,
	MUSHROOM_SPORE,
};

struct mushroom_conf_port {
	int port;
	char *address;
};

struct mushroom_conf {
	enum mushroom_log_level log_level;

	enum mushroom_node_mode mode;

	struct mushroom_conf_port gossip;
	struct mushroom_conf_port initial_node;
	struct mushroom_conf_port api;
};

void mushroom_conf_default(struct mushroom_conf *conf);
bool mushroom_conf_from_args(struct mushroom_conf *conf, int argc, char *argv[]);
void mushroom_conf_log(struct mushroom_conf *conf);
