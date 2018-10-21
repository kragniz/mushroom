#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <uv.h>

#define PROJECT_NAME "mushroom"

#define CHECK(r, msg)                                                          \
	if (r < 0) {                                                           \
		fprintf(stderr, "%s: %s\n", msg, uv_strerror(r));              \
		exit(1);                                                       \
	}

static uv_loop_t *uv_loop;
static uv_udp_t server;

static void on_recv(uv_udp_t *handle, ssize_t nread, const uv_buf_t *rcvbuf,
		    const struct sockaddr *addr, unsigned flags)
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

int main(int argc, char **argv)
{
	int status;
	struct sockaddr_in addr;
	uv_loop = uv_default_loop();

	status = uv_udp_init(uv_loop, &server);
	CHECK(status, "init");

	uv_ip4_addr("0.0.0.0", 11000, &addr);

	status = uv_udp_bind(&server, (const struct sockaddr *)&addr, 0);
	CHECK(status, "bind");

	status = uv_udp_recv_start(&server, on_alloc, on_recv);
	CHECK(status, "recv");

	if (argc != 1) {
		printf("%s takes no arguments.\n", argv[0]);
		return 1;
	}
	printf("This is project %s.\n", PROJECT_NAME);

	return uv_run(uv_loop, UV_RUN_DEFAULT);
}
