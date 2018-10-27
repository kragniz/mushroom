#pragma once

#include <stdbool.h>

struct mushroom_conf {
	int gossip_port;
	char *gossip_address;
};

void mushroom_conf_default(struct mushroom_conf *conf);
bool mushroom_conf_from_args(struct mushroom_conf *conf, int argc, char *argv[]);
void mushroom_conf_log(struct mushroom_conf *conf);