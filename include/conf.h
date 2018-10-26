#pragma once

struct mushroom_conf {
	int gossip_port;
};

void mushroom_conf_default(struct mushroom_conf *conf);
void mushroom_conf_from_args(struct mushroom_conf *conf, int argc, char *argv[]);