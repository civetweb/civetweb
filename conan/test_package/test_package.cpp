/*
 * Copyright (c) 2018-2020 the CivetWeb developers
 * MIT License
 */

/* Simple demo of a REST callback. */
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "civetweb.h"

#define PORT "8080"
#define HOST_INFO "http://localhost:8080"
#define EXAMPLE_URI "/example"

static int exitNow = 0;

static int
ExampleGET(struct mg_connection* conn)
{
    mg_send_http_ok(conn, "text/plain", 10);
    exitNow = 1;
	return 200;
}

static int
ExampleHandler(struct mg_connection *conn, void *cbdata)
{
	const struct mg_request_info *ri = mg_get_request_info(conn);
	(void)cbdata; /* currently unused */

	if (0 == strcmp(ri->request_method, "GET")) {
		return ExampleGET(conn);
	}
	return 405;
}

int
log_message(const struct mg_connection *conn, const char *message)
{
	puts(message);
	return 1;
}

int
main(int argc, char *argv[])
{
	struct mg_callbacks callbacks;
	struct mg_context *ctx;
	time_t start_t;
	time_t end_t;
	double diff_t;
	int err = 0;

	mg_init_library(0);

	/* Callback will print error messages to console */
	memset(&callbacks, 0, sizeof(callbacks));
	callbacks.log_message = log_message;

	/* Start CivetWeb web server */
	ctx = mg_start(&callbacks, 0, NULL);

	/* Check return value: */
	if (ctx == NULL) {
		fprintf(stderr, "Cannot start CivetWeb - mg_start failed.\n");
		return EXIT_FAILURE;
	}

	/* Add handler EXAMPLE_URI, to explain the example */
	mg_set_request_handler(ctx, EXAMPLE_URI, ExampleHandler, 0);

	/* Show sone info */
	printf("Start example: %s%s\n", HOST_INFO, EXAMPLE_URI);


	/* Wait until the server should be closed */
	time(&start_t);
	while (!exitNow) {
#ifdef _WIN32
		Sleep(1000);
#else
		sleep(1);
#endif
        time(&end_t);
		diff_t = difftime(end_t, start_t);
		if (diff_t > 3.0) {
			break;
		}
	}

	/* Stop the server */
	mg_stop(ctx);
	return EXIT_SUCCESS;
}
