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

enum opts {
	OPT_GOSSIP_PORT = 255,
	OPT_GOSSIP_ADDRESS,
	OPT_INITIAL_NODE_ADDRESS,
	OPT_INITIAL_NODE_PORT,
	OPT_API_ADDRESS,
	OPT_API_PORT,
};

static int parse_port_option(const char *optarg)
{
	assert(optarg != NULL);
	errno = 0;
	uintmax_t num = strtoumax(optarg, NULL, 10);
	if (errno != 0) {
		mushroom_log_fatal(strerror(errno));
	}

	if (num <= 0) {
		mushroom_log_fatal("invalid port number: %s", optarg);
	}

	return (int)num;
}

void mushroom_conf_default(struct mushroom_conf *conf)
{
	conf->mode = MUSHROOM_GROW;
	conf->gossip_port = 6868;
	conf->gossip_address = "127.0.0.1";

	conf->initial_node_port = 6767;
	conf->initial_node_address = NULL;

	conf->api_port = 6969;
	conf->api_address = "127.0.0.1";
}

void mushroom_conf_log(struct mushroom_conf *conf)
{
	char *mode;
	switch (conf->mode) {
	case MUSHROOM_GROW:
		mode = "grow";
		break;
	case MUSHROOM_SPORE:
		mode = "spore";
		break;
	default:
		mode = "unknown";
		break;
	}
	mushroom_log_info("mode: %s", mode);
	mushroom_log_info("gossip_port: %i", conf->gossip_port);
	mushroom_log_info("gossip_address: %s", conf->gossip_address);
	mushroom_log_info("initial_node_port: %i", conf->initial_node_port);
	mushroom_log_info("initial_node_address: %s", conf->initial_node_address);
	mushroom_log_info("api_port: %i", conf->api_port);
	mushroom_log_info("api_address: %s", conf->api_address);
}

bool mushroom_conf_from_args(struct mushroom_conf *conf, int argc, char *argv[])
{
	const char *short_opt = "h";
	static struct option long_opt[] = {
		{ "help", no_argument, NULL, 'h' },
		{ "gossip-port", required_argument, NULL, OPT_GOSSIP_PORT },
		{ "gossip-address", required_argument, NULL, OPT_GOSSIP_ADDRESS },
		{ "initial-node-port", required_argument, NULL, OPT_INITIAL_NODE_PORT },
		{ "initial-node-address", required_argument, NULL, OPT_INITIAL_NODE_ADDRESS },
		{ "api-port", required_argument, NULL, OPT_API_PORT },
		{ "api-address", required_argument, NULL, OPT_API_ADDRESS },
		{ 0 }
	};

	/* reset getopt */
	optind = 0;

	int c;
	while ((c = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1) {
		switch (c) {
		case -1:
		case 0:
			return false;
		case OPT_GOSSIP_PORT:
			conf->gossip_port = parse_port_option(optarg);
			break;
		case OPT_GOSSIP_ADDRESS:
			assert(optarg != NULL);
			conf->gossip_address = optarg;
			break;
		case OPT_INITIAL_NODE_PORT:
			conf->initial_node_port = parse_port_option(optarg);
			break;
		case OPT_INITIAL_NODE_ADDRESS:
			assert(optarg != NULL);
			conf->initial_node_address = optarg;
			break;
		case OPT_API_PORT:
			conf->api_port = parse_port_option(optarg);
			break;
		case OPT_API_ADDRESS:
			assert(optarg != NULL);
			conf->api_address = optarg;
			break;
		case 'h':
			printf("Usage: %s [OPTIONS]\n", argv[0]);
			printf("  -h, --help              print this help and exit\n");
			printf("  --gossip-port           port the gossip server listens on\n");
			printf("  --gossip-address        address the gossip server listens on\n");
			printf("  --initial-node-port     port the initial node gossip server listens on\n");
			printf("  --initial-node-address  address the initial node gossip server listens on\n");
			printf("  --api-port              port the api server listens on\n");
			printf("  --api-address           address the api server listens on\n");
			return false;
		default:
			return false;
		}
	}

	if (optind < argc) {
		while (optind < argc) {
			char *arg = argv[optind++];
			if (strncmp(arg, "spore", strlen(arg)) == 0) {
				conf->mode = MUSHROOM_SPORE;
				break;
			} else if (strncmp(arg, "grow", strlen(arg)) == 0) {
				conf->mode = MUSHROOM_GROW;
				break;
			} else {
				mushroom_log_error("invalid node mode: %s", arg);
				return false;
			}
		}
	}

	return true;
}
