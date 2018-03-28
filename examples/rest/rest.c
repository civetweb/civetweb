/*
* Copyright (c) 2018 the CivetWeb developers
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
#include "cJSON.h"


#ifdef NO_SSL
#ifdef USE_IPV6
#define PORT "[::]:8888,8884"
#else
#define PORT "8888,8884"
#endif
#else
#ifdef USE_IPV6
#define PORT "[::]:8888r,[::]:8843s,8884"
#else
#define PORT "8888r,8843s,8884"
#endif
#endif

#define EXAMPLE_URI "/example"
#define EXIT_URI "/exit"
int exitNow = 0;


static int
SendJSON(struct mg_connection *conn, cJSON *json_obj)
{
	char *json_str = cJSON_PrintUnformatted(json_obj);
	size_t json_str_len = strlen(json_str);

	/* Send HTTP message header */
	mg_send_http_ok(conn, "application/json; charset=utf-8", json_str_len);

	/* Send HTTP message content */
	mg_write(conn, json_str, json_str_len);

	/* Free string allocated by cJSON_Print* */
	cJSON_free(json_str);

	return (int)json_str_len;
}


int
ExampleHandler(struct mg_connection *conn, void *cbdata)
{
	static unsigned request = 0;

	const struct mg_request_info *ri = mg_get_request_info(conn);
	cJSON *obj;

	if (0 != strcmp(ri->request_method, "GET")) {
		/* this is not a GET request */
		mg_send_http_error(conn, 405, "Only GET method supported");
		return 405;
	}

	
	obj = cJSON_CreateObject();

	if (!obj) {
		/* insufficient memory? */
		mg_send_http_error(conn, 500, "Server error");
		return 500;
	}


	cJSON_AddStringToObject(obj, "version", CIVETWEB_VERSION);
	cJSON_AddNumberToObject(obj, "request", ++request);
	SendJSON(conn, obj);
	cJSON_Delete(obj);

	return 200;
}


int
ExitHandler(struct mg_connection *conn, void *cbdata)
{
	mg_printf(conn,
	          "HTTP/1.1 200 OK\r\nContent-Type: "
	          "text/plain\r\nConnection: close\r\n\r\n");
	mg_printf(conn, "Server will shut down.\n");
	mg_printf(conn, "Bye!\n");
	exitNow = 1;
	return 1;
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
	const char *options[] = {"listening_ports",
	                         PORT,
	                         "request_timeout_ms",
	                         "10000",
	                         "error_log_file",
	                         "error.log",
#ifndef NO_SSL
	                         "ssl_certificate",
	                         "../../resources/cert/server.pem",
	                         "ssl_protocol_version",
	                         "3",
	                         "ssl_cipher_list",
	                         "DES-CBC3-SHA:AES128-SHA:AES128-GCM-SHA256",
#endif
	                         "enable_auth_domain_check",
	                         "no",
	                         0};

	struct mg_callbacks callbacks;
	struct mg_context *ctx;
	int err = 0;

/* Check if libcivetweb has been built with all required features. */
#ifndef NO_SSL
	if (!mg_check_feature(2)) {
		fprintf(stderr,
		        "Error: Embedded example built with SSL support, "
		        "but civetweb library build without.\n");
		err = 1;
	}


	mg_init_library(MG_FEATURES_SSL);

#else
	mg_init_library(0);

#endif
	if (err) {
		fprintf(stderr, "Cannot start CivetWeb - inconsistent build.\n");
		return EXIT_FAILURE;
	}


	/* Callback will print error messages to console */
	memset(&callbacks, 0, sizeof(callbacks));
	callbacks.log_message = log_message;

	/* Start CivetWeb web server */
	ctx = mg_start(&callbacks, 0, options);

	/* Check return value: */
	if (ctx == NULL) {
		fprintf(stderr, "Cannot start CivetWeb - mg_start failed.\n");
		return EXIT_FAILURE;
	}

	/* Add handler EXAMPLE_URI, to explain the example */
	mg_set_request_handler(ctx, EXAMPLE_URI, ExampleHandler, 0);
	mg_set_request_handler(ctx, EXIT_URI, ExitHandler, 0);


	/* Wait until the server should be closed */
	while (!exitNow) {
#ifdef _WIN32
		Sleep(1000);
#else
		sleep(1);
#endif
	}

	/* Stop the server */
	mg_stop(ctx);

	printf("Server stopped.\n");
	printf("Bye!\n");

	return EXIT_SUCCESS;
}
