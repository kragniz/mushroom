#include <stdlib.h>

#include <uv.h>

#include "gossip_reader.h"
#include "gossip_server.h"
#include "log.h"

static void on_recv(uv_udp_t *handle,
		    ssize_t nread,
		    const uv_buf_t *rcvbuf,
		    const struct sockaddr *addr,
		    unsigned flags)
{
	mushroom_log_debug("received gossip message");
	if (nread > 0) {
		mushroom_gossip_message_table_t msg = mushroom_gossip_message_as_root(rcvbuf->base);
		if (msg == NULL) {
			mushroom_log_error("gossip message couldn't be parsed");
		} else {
			uint32_t from = mushroom_gossip_message_from(msg);
			mushroom_log_debug("gossip message was from %d", from);
		}
	}
	free(rcvbuf->base);
}

static void on_alloc(uv_handle_t *client, size_t suggested_size, uv_buf_t *buf)
{
	buf->base = malloc(suggested_size);
	buf->len = suggested_size;
}

struct mushroom_gossip_server *
mushroom_gossip_server_new(uv_loop_t *loop, const char *addr, int port)
{
	struct mushroom_gossip_server *server = calloc(1, sizeof(*server));
	server->addr = calloc(1, sizeof(*server->addr));
	server->server = calloc(1, sizeof(*server->server));

	server->loop = loop;

	int err = uv_udp_init(server->loop, server->server);
	if (err < 0) {
		mushroom_log_fatal("init: %s", uv_strerror(err));
	}

	mushroom_log_debug("created mushroom_gossip_server:\t%p", server);

	uv_ip4_addr(addr, port, server->addr);
	mushroom_log_info("gossip server listening on %s:%i", addr, port);

	return server;
}

void mushroom_gossip_server_start(struct mushroom_gossip_server *server)
{
	int err = uv_udp_bind(server->server, (const struct sockaddr *)server->addr, 0);
	if (err < 0) {
		mushroom_log_fatal("bind: %s %s", uv_strerror(err), "toot");
	}

	err = uv_udp_recv_start(server->server, on_alloc, on_recv);
	if (err < 0) {
		mushroom_log_fatal("start: %s", uv_strerror(err));
	}
}