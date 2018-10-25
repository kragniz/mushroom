#include <stdlib.h>

#include <uv.h>

#include "gossip_server.h"
#include "log.h"

static void on_recv(uv_udp_t *handle,
		    ssize_t nread,
		    const uv_buf_t *rcvbuf,
		    const struct sockaddr *addr,
		    unsigned flags)
{
	if (nread > 0) {
		printf("%lu\n", nread);
		printf("%s", rcvbuf->base);
	}
	printf("free  :%lu %p\n", rcvbuf->len, (void *)rcvbuf->base);
	free(rcvbuf->base);
}

static void on_alloc(uv_handle_t *client, size_t suggested_size, uv_buf_t *buf)
{
	buf->base = malloc(suggested_size);
	buf->len = suggested_size;
	printf("malloc:%lu %p\n", buf->len, (void *)buf->base);
}

struct mushroom_gossip_server *mushroom_gossip_server_new(uv_loop_t *loop, int port)
{
	struct mushroom_gossip_server *server = calloc(1, sizeof(*server));
	server->addr = calloc(1, sizeof(server->addr));
	server->server = calloc(1, sizeof(server->server));

	server->loop = loop;

	int err = uv_udp_init(server->loop, server->server);
	if (err < 0) {
		fprintf(stderr, "init: %s\n", uv_strerror(err));
		exit(1);
	}

	char *addr = "0.0.0.0";
	uv_ip4_addr(addr, port, server->addr);
	mushroom_log_debug("listening on %s:%i", addr, port);

	return server;
}

void mushroom_gossip_server_start(struct mushroom_gossip_server *server)
{
	int err = uv_udp_bind(server->server, (const struct sockaddr *)server->addr, 0);
	if (err < 0) {
		fprintf(stderr, "bind: %s\n", uv_strerror(err));
		exit(1);
	}

	err = uv_udp_recv_start(server->server, on_alloc, on_recv);
	if (err < 0) {
		fprintf(stderr, "start: %s\n", uv_strerror(err));
		exit(1);
	}
}