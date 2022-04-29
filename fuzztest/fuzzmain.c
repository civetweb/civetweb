/********************************************************/
/*                                                      */
/*   FUZZ TEST for civetweb.c                           */
/*                                                      */
/*   Copyright (c) 2015-2020 the CivetWeb developers    */
/*                                                      */
/*   This file contains test code for fuzz tests.       */
/*   It should not be used in production code.          */
/*                                                      */
/********************************************************/

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
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
typedef int SOCKET;
#define closesocket(a) (close(a))

#endif // not _WIN32


/* Port configuration */
unsigned short PORT_NUM_HTTP = 0; /* set dynamically */


#define TESTabort()                                                            \
	{                                                                          \
		fprintf(stderr, "!!! Precondition in test environment not met !!!\n"); \
		fprintf(stderr, "!!! aborting fuzz test in line %u !!!", __LINE__);    \
		abort();                                                               \
	}


static uint64_t call_count = 0;


/********************************************************/
/* Init CivetWeb server ... test with mock client       */
/********************************************************/
#if defined(TEST_FUZZ1) || defined(TEST_FUZZ2)

static struct mg_context *ctx = 0;
static const char *OPTIONS[] = {"listening_ports",
                                "0", /* port: auto */
                                "document_root",
                                "fuzztest/docroot",
                                NULL,
                                NULL};

static void
civetweb_exit(void)
{
	printf("CivetWeb server exit\n");
	mg_stop(ctx);
	ctx = 0;
	test_sleep(5);
}


static void
civetweb_init(void)
{
	struct mg_callbacks callbacks;
	struct mg_server_port ports[8];
	memset(&callbacks, 0, sizeof(callbacks));
	memset(&ports, 0, sizeof(ports));

	ctx = mg_start(&callbacks, 0, OPTIONS);

	if (!ctx) {
		fprintf(stderr, "\nCivetWeb test server failed to start\n");
		TESTabort();
	}

	int ret = mg_get_server_ports(ctx, 8, ports);
	if (ret != 1) {
		fprintf(stderr,
		        "\nCivetWeb test server: cannot determine port number\n");
		TESTabort();
	}
	if (ports[0].is_ssl != 0) {
		fprintf(stderr,
		        "\nCivetWeb fuzz test works on HTTP, not HTTPS.\n"
		        "TLS library should be fuzzed separately.\n");
		TESTabort();
	}
	PORT_NUM_HTTP = ports[0].port;

	printf("CivetWeb server running on port %i\n", (int)PORT_NUM_HTTP);

	/* Give server 5 seconds to start, before flooding with requests.
	 * Don't know if this is required for fuzz-tests, but it was helpful
	 * when testing starting/stopping the server multiple times in test
	 * container environments. */
	test_sleep(5);
	atexit(civetweb_exit);
}


#if defined(TEST_FUZZ1)
static int
test_civetweb_client(const char *server,
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
		civetweb_init();
	}
	call_count++;

	if (size < sizeof(URI)) {
		memcpy(URI, data, size);
		URI[size] = 0;
	} else {
		return 1;
	}

	return test_civetweb_client("127.0.0.1", PORT_NUM_HTTP, 0, URI);
}
#endif


#if defined(TEST_FUZZ2)
static int
LLVMFuzzerTestOneInput_REQUEST(const uint8_t *data, size_t size)
{
	if (call_count == 0) {
		civetweb_init();
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
	sin.sin_port = htons(PORT_NUM_HTTP);
	r = connect(sock, (struct sockaddr *)&sin, sizeof(sin));
	if (r != 0) {
		r = errno;
		fprintf(stderr, "Error: Cannot connect [%s]\n", strerror(r));
		closesocket(sock);
		return 1;
	}

	char trash[1024];
	r = send(sock, data, size, 0);
	if (r != (int)size) {
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
	return 0;
}
#endif

#endif // defined(TEST_FUZZ1) || defined(TEST_FUZZ2)


/********************************************************/
/* Init mock server ... test with CivetWeb client       */
/********************************************************/
#if defined(TEST_FUZZ3)

struct tcp_func_prm {
	SOCKET sock;
};

struct tRESPONSE {
	char data[4096];
	size_t size;
} RESPONSE;


volatile int mock_server_stop_flag = 0;

static void
mock_server_exit(void)
{
	printf("MOCK server exit\n");
	mock_server_stop_flag = 1;
	test_sleep(5);
}


static void *
mock_server_thread(void *arg)
{
	char req[1024 * 16];
	SOCKET svr = (SOCKET)(-1);

	/* Get thread parameters and free arg */
	{
		struct tcp_func_prm *ptcp_func_prm = (struct tcp_func_prm *)arg;
		svr = ptcp_func_prm->sock;
		free(arg);
	}

	mock_server_stop_flag = 0;
	printf("MOCK server ready, sock %i\n", svr);

next_request:
	while (!mock_server_stop_flag) {
		struct sockaddr_in cliadr;
		socklen_t adrlen = sizeof(cliadr);
		int buf_filled = 0;
		int req_ready = 0;

		memset(&cliadr, 0, sizeof(cliadr));

		SOCKET cli = accept(svr, (struct sockaddr *)&cliadr, &adrlen);

		if (cli == -1) {
			int er = errno;
			fprintf(stderr, "Error: Accept failed [%s]\n", strerror(er));
			test_sleep(1);
			goto next_request;
		}

		/* Read request */
		do {
			int r =
			    recv(cli, req + buf_filled, sizeof(req) - buf_filled - 1, 0);
			if (r > 0) {
				buf_filled += r;
				req[buf_filled] = 0;
				if (strstr(req, "\r\n\r\n") != NULL) {
					req_ready = 1;
				}
			} else {
				/* some error */
				int er = errno;
				fprintf(stderr, "Error: Recv failed [%s]\n", strerror(er));
				test_sleep(1);
				goto next_request;
			}
		} while (!req_ready);

		/* Request is complete here.
		 * Now send response */
		send(cli, RESPONSE.data, RESPONSE.size, MSG_NOSIGNAL);

		/* Close connection. */
		shutdown(cli, SHUT_RDWR);
		closesocket(cli);
	}
	return 0;
}


static void
mock_server_init(void)
{
	int r;
	int bind_success = 0;
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 6);
	if (sock == -1) {
		r = errno;
		fprintf(stderr, "Error: Cannot create socket [%s]\n", strerror(r));
		TESTabort();
	}

	for (PORT_NUM_HTTP = 1024; PORT_NUM_HTTP != 0; PORT_NUM_HTTP++) {
		struct sockaddr_in sin;
		memset(&sin, 0, sizeof(sin));
		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = inet_addr("127.0.0.1");
		sin.sin_port = htons(PORT_NUM_HTTP);
		r = bind(sock, (struct sockaddr *)&sin, sizeof(sin));
		if (r == 0) {
			bind_success = 1;
			break;
		}
		r = errno;
		fprintf(stderr, "Warning: Cannot bind [%s]\n", strerror(r));
	}

	if (!bind_success) {
		fprintf(stderr, "Error: Cannot bind to any port\n");
		closesocket(sock);
		TESTabort();
	}

	printf("MOCK server running on port %i\n", (int)PORT_NUM_HTTP);

	r = listen(sock, 128);
	if (r != 0) {
		r = errno;
		fprintf(stderr, "Error: Cannot listen [%s]\n", strerror(r));
		closesocket(sock);
		TESTabort();
	}

	pthread_t thread_id;
	pthread_attr_t attr;
	int result;
	struct tcp_func_prm *thread_prm;

	thread_prm = (struct tcp_func_prm *)malloc(sizeof(struct tcp_func_prm));
	if (!thread_prm) {
		fprintf(stderr, "Error: Out of memory\n");
		closesocket(sock);
		TESTabort();
	}
	thread_prm->sock = sock;

	(void)pthread_attr_init(&attr);
	(void)pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	result = pthread_create(&thread_id,
	                        &attr,
	                        mock_server_thread,
	                        (void *)thread_prm);
	(void)pthread_attr_destroy(&attr);
	if (result != 0) {
		r = errno;
		fprintf(stderr, "Error: Cannot create thread [%s]\n", strerror(r));
		closesocket(sock);
		TESTabort();
	}

	test_sleep(3);
	atexit(mock_server_exit);
}


static int
LLVMFuzzerTestOneInput_RESPONSE(const uint8_t *data, size_t size)
{
	if (call_count == 0) {
		mock_server_init();
	}
	call_count++;

	if (size > sizeof(RESPONSE.data)) {
		return 1;
	}

	memcpy(RESPONSE.data, data, size);
	RESPONSE.size = size;

	char errbuf[256];

	struct mg_connection *conn = mg_connect_client(
	    "127.0.0.1", PORT_NUM_HTTP, 0, errbuf, sizeof(errbuf));
	if (!conn) {
		printf("Connect error: %s\n", errbuf);
		test_sleep(1);
		return 1;
	}
	mg_printf(conn, "GET / HTTP/1.0\r\n\r\n");

	int r = mg_get_response(conn, errbuf, sizeof(errbuf), 1000);
	const struct mg_response_info *ri = mg_get_response_info(conn);

	(void)r;
	(void)ri;

	mg_close_connection(conn);

	return 0;
}

#endif // defined(TEST_FUZZ3)

/********************************************************/
/* MAIN for fuzztest                                    */
/********************************************************/
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size);
// warning: no previous prototype for function 'LLVMFuzzerTestOneInput'

int
LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
#if defined(TEST_FUZZ1)
	/* fuzz target 1: different URI for HTTP/1 server */
	return LLVMFuzzerTestOneInput_URI(data, size);
#elif defined(TEST_FUZZ2)
	/* fuzz target 2: different requests for HTTP/1 server */
	return LLVMFuzzerTestOneInput_REQUEST(data, size);
#elif defined(TEST_FUZZ3)
	/* fuzz target 3: different responses for HTTP/1 client */
	return LLVMFuzzerTestOneInput_RESPONSE(data, size);
#elif defined(TEST_FUZZ4)
#error "Only useful in HTTP/2 feature branch"
	/* fuzz target 4: different requests for HTTP/2 server */
	return LLVMFuzzerTestOneInput_REQUEST_HTTP2(data, size);
#elif defined(TEST_FUZZ5)
	/* fuzz target 5: calling an internal server test function,
	 *                bypassing network sockets */
	return LLVMFuzzerTestOneInput_process_new_connection(data, size);
#else
/* planned targets */
#error "Unknown fuzz target"
#endif
}
