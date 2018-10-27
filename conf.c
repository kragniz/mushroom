#include <assert.h>
#include <errno.h>
#include <getopt.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "conf.h"
#include "log.h"

void mushroom_conf_default(struct mushroom_conf *conf)
{
	conf->gossip_port = 6868;
	conf->gossip_address = "127.0.0.1";
}

void mushroom_conf_log(struct mushroom_conf *conf)
{
	mushroom_log_info("gossip_port=%i", conf->gossip_port);
	mushroom_log_info("gossip_address=%s", conf->gossip_address);
}

bool mushroom_conf_from_args(struct mushroom_conf *conf, int argc, char *argv[])
{
	const char *short_opt = "hg:p";
	static struct option long_opt[] = { { "help", no_argument, NULL, 'h' },
					    { "gossip-port", required_argument, NULL, 'g' },
					    { "gossip-address", required_argument, NULL, 'a' },
					    { 0 } };

	uintmax_t num = 0;

	/* reset getopt */
	optind = 0;

	int c;
	while ((c = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1) {
		switch (c) {
		case -1:
		case 0:
			return false;
		case 'g':
			assert(optarg != NULL);
			errno = 0;
			num = strtoumax(optarg, NULL, 10);
			if (errno != 0) {
				mushroom_log_fatal(strerror(errno));
				return false;
			}

			if (num <= 0) {
				mushroom_log_fatal("invalid port number: %s", optarg);
				return false;
			}

			conf->gossip_port = (int)num;
			break;
		case 'a':
			assert(optarg != NULL);
			conf->gossip_address = optarg;
			break;
		case 'h':
			printf("Usage: %s [OPTIONS]\n", argv[0]);
			printf("  -h, --help            print this help and exit\n");
			printf("  -g, --gossip-port     port the gossip server listens on\n");
			printf("  --gossip-address      address the gossip server listens on\n");
			return false;
		default:
			return false;
		}
	}
	return true;
}