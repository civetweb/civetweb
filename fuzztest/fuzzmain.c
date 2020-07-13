#include "civetweb.h"
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
#error "Currently not supported"
#else

#include <unistd.h>
#define test_sleep(x) (sleep(x))
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>
typedef int SOCKET;
#define closesocket(a) (close(a))

#endif


static uint64_t call_count = 0;

static struct mg_context *ctx;
static const char *OPTIONS[] = {"listening_ports",
                                "8080,8443s",
                                "document_root",
                                "fuzztest/docroot",
                                "ssl_certificate",
                                "resources/cert/server.pem",
                                NULL,
                                NULL};


static void
init_civetweb(void)
{
	struct mg_callbacks callbacks;
	memset(&callbacks, 0, sizeof(callbacks));

	ctx = mg_start(&callbacks, 0, OPTIONS);

	if (!ctx) {
		fprintf(stderr, "\nCivetWeb test server failed to start\n");
		abort();
	}

	/* Give server 5 seconds to start, before flooding with requests.
	 * Don't know if this is required for fuzz-tests, but it was helpful
	 * when testing starting/stopping the server multiple times in test
	 * container environments. */
	test_sleep(5);
}


static int
test_http_request(const char *server,
                  uint16_t port,
                  int use_ssl,
                  const char *uri)
{
	/* Client var */
	struct mg_connection *client;
	char client_err_buf[256];
	char client_data_buf[4096];
	const struct mg_response_info *client_ri;
	int64_t data_read;
	int r;

	client = mg_connect_client(
	    server, port, use_ssl, client_err_buf, sizeof(client_err_buf));

	if ((client == NULL) || (0 != strcmp(client_err_buf, ""))) {
		fprintf(stderr,
		        "%s connection to server [%s] port [%u] failed: [%s]\n",
		        use_ssl ? "HTTPS" : "HTTP",
		        server,
		        port,
		        client_err_buf);
		if (client) {
			mg_close_connection(client);
		}

		/* In heavy fuzz testing, sometimes we run out of available sockets.
		 * Wait for some seconds, and retry. */
		test_sleep(5);

		/* retry once */
		client = mg_connect_client(
		    server, port, use_ssl, client_err_buf, sizeof(client_err_buf));
		if (!client) {
			fprintf(stderr, "Retry: error\n");
			return 1;
		}
		fprintf(stderr, "Retry: success\n");
	}

	mg_printf(client, "GET %s HTTP/1.0\r\n\r\n", uri);

	r = mg_get_response(client, client_err_buf, sizeof(client_err_buf), 10000);

	if ((r < 0) || (0 != strcmp(client_err_buf, ""))) {
		mg_close_connection(client);
		return 1;
	}

	client_ri = mg_get_response_info(client);
	if (client_ri == NULL) {
		mg_close_connection(client);
		return 1;
	}

	data_read = 0;
	while (data_read < client_ri->content_length) {
		/* store the first sizeof(client_data_buf) bytes
		 * of the HTTP response. */
		r = mg_read(client,
		            client_data_buf + data_read,
		            sizeof(client_data_buf) - (size_t)data_read);
		if (r > 0) {
			data_read += r;
		}

		/* buffer filled? */
		if (sizeof(client_data_buf) == (size_t)data_read) {
			/* ignore the rest */
			while (r > 0) {
				char trash[1024];
				r = mg_read(client, trash, sizeof(trash));
			}
			break;
		}
	}

	/* Nothing left to read */
	r = mg_read(client, client_data_buf, sizeof(client_data_buf));
	if (r != 0) {
		mg_close_connection(client);
		return 1;
	}

	mg_close_connection(client);
	return 0;
}


static int
LLVMFuzzerTestOneInput_URI(const uint8_t *data, size_t size)
{
	static char URI[1024 * 64]; /* static, to avoid stack overflow */

	if (call_count == 0) {
		memset(URI, 0, sizeof(URI));
		init_civetweb();
	}
	call_count++;

	if (size < sizeof(URI)) {
		memcpy(URI, data, size);
		URI[size] = 0;
	} else {
		return 1;
	}

	printf("URI: %s\n", URI);

	return test_http_request("127.0.0.1", 8080, 0, URI);
}


static int
LLVMFuzzerTestOneInput_REQUEST(const uint8_t *data, size_t size)
{
	if (call_count == 0) {
		init_civetweb();
	}
	call_count++;

	int r;
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 6);
	if (sock == -1) {
		r = errno;
		fprintf(stderr, "Error: Cannot create socket [%s]\n", strerror(r));
		return 1;
	}
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	sin.sin_port = htons(8080);
	r = connect(sock, (struct sockaddr *)&sin, sizeof(sin));
	if (r != 0) {
		r = errno;
		fprintf(stderr, "Error: Cannot connect [%s]\n", strerror(r));
		closesocket(sock);
		return 1;
	}

	char trash[1024];
	r = send(sock, data, size, 0);
	if (r != size) {
		fprintf(stderr, "Warning: %i bytes sent (TODO: Repeat)\n", r);
	}

	int data_read = 0;
	while ((r = recv(sock, trash, sizeof(trash), 0)) > 0) {
		data_read += r;
	};

	shutdown(sock, SHUT_RDWR);
	closesocket(sock);

	static int max_data_read = 0;
	if (data_read > max_data_read) {
		max_data_read = data_read;
		printf("GOT data: %i\n", data_read);
	}
}

int
LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
#if defined(TEST_FUZZ1)
	/* fuzz target 1: different URI for HTTP/1 server */
	return LLVMFuzzerTestOneInput_URI(data, size);
#elif defined(TEST_FUZZ2)
	/* fuzz target 2: different requests for HTTP/1 server */
	return LLVMFuzzerTestOneInput_REQUEST(data, size);
#else
/* planned targets */
/* fuzz target 3: different responses for HTTP/1 client */
/* fuzz target 4: different requests for HTTP/2 server */
/* fuzz target 5: calling an internal server test function,
 *                bypassing network sockets */
#error "Unknown fuzz target"
#endif
}
