#include <assert.h>
#include <errno.h>
#include <getopt.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "conf.h"
#include "log.h"

void mushroom_conf_default(struct mushroom_conf *conf)
{
	conf->gossip_port = 6868;
}

void mushroom_conf_from_args(struct mushroom_conf *conf, int argc, char *argv[])
{
	int c;
	const char *short_opt = "hg:p";
	static struct option long_opt[] = { { "help", no_argument, NULL, 'h' },
					    { "gossip-port", required_argument, NULL, 'g' },
					    { NULL, 0, NULL, 0 } };

	uintmax_t num = 0;

	while ((c = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1) {
		switch (c) {
		case -1:
		case 0:
			break;
		case 'g':
			assert(optarg != NULL);
			num = strtoumax(optarg, NULL, 10);
			if (num == UINTMAX_MAX && errno == ERANGE)
				mushroom_log_error("bad value for --gossip-port");
			conf->gossip_port = (int)num;
			break;
		case 'h':
			printf("Usage: %s [OPTIONS]\n", argv[0]);
			printf("  -h, --help            print this help and exit\n");
			printf("  -v, --verbose         enable more verbose logging\n");
		default:
			return;
		}
	}
}