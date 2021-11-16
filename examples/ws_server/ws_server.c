#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /* for sleep() */

#include "civetweb.h"


/* Global options for this example. */
static const char WS_URL[] = "/wsURL";
static const char *SERVER_OPTIONS[] =
    {"listening_ports", "8081", "num_threads", "10", NULL, NULL};

/* Define websocket sub-protocols. */
/* This must be static data, available between mg_start and mg_stop. */
static const char subprotocol_bin[] = "Company.ProtoName.bin";
static const char subprotocol_json[] = "Company.ProtoName.json";
static const char *subprotocols[] = {subprotocol_bin, subprotocol_json, NULL};
static struct mg_websocket_subprotocols wsprot = {2, subprotocols};


/* Exit flag for the server */
volatile int g_exit = 0;


/* User defined data structure for websocket client context. */
struct tClientContext {
	uint32_t connectionNumber;
	uint32_t demo_var;
};


/* Handler for new websocket connections. */
static int
ws_connect_handler(const struct mg_connection *conn, void *user_data)
{
	(void)user_data; /* unused */

	/* Allocate data for websocket client context, and initialize context. */
	struct tClientContext *wsCliCtx =
	    (struct tClientContext *)calloc(1, sizeof(struct tClientContext));
	if (!wsCliCtx) {
		/* reject client */
		return 1;
	}
	static uint32_t connectionCounter = 0; /* Example data: client number */
	wsCliCtx->connectionNumber = __sync_add_and_fetch(&connectionCounter, 1);
	mg_set_user_connection_data(
	    conn, wsCliCtx); /* client context assigned to connection */

	/* DEBUG: New client connected (but not ready to receive data yet). */
	const struct mg_request_info *ri = mg_get_request_info(conn);
	printf("Client %u connected with subprotocol: %s\n",
	       wsCliCtx->connectionNumber,
	       ri->acceptedWebSocketSubprotocol);

	return 0;
}


/* Handler indicating the client is ready to receive data. */
static void
ws_ready_handler(struct mg_connection *conn, void *user_data)
{
	(void)user_data; /* unused */

	/* Get websocket client context information. */
	struct tClientContext *wsCliCtx =
	    (struct tClientContext *)mg_get_user_connection_data(conn);
	const struct mg_request_info *ri = mg_get_request_info(conn);
	(void)ri; /* in this example, we do not need the request_info */

	/* Send "hello" message. */
	const char *hello = "{}";
	size_t hello_len = strlen(hello);
	mg_websocket_write(conn, MG_WEBSOCKET_OPCODE_TEXT, hello, hello_len);

	/* DEBUG: New client ready to receive data. */
	printf("Client %u ready to receive data\n", wsCliCtx->connectionNumber);
}


/* Handler indicating the client sent data to the server. */
static int
ws_data_handler(struct mg_connection *conn,
                int opcode,
                char *data,
                size_t datasize,
                void *user_data)
{
	(void)user_data; /* unused */

	/* Get websocket client context information. */
	struct tClientContext *wsCliCtx =
	    (struct tClientContext *)mg_get_user_connection_data(conn);
	const struct mg_request_info *ri = mg_get_request_info(conn);
	(void)ri; /* in this example, we do not need the request_info */

	/* DEBUG: Print data received from client. */
	const char *messageType = "";
	switch (opcode & 0xf) {
	case MG_WEBSOCKET_OPCODE_TEXT:
		messageType = "text";
		break;
	case MG_WEBSOCKET_OPCODE_BINARY:
		messageType = "binary";
		break;
	case MG_WEBSOCKET_OPCODE_PING:
		messageType = "ping";
		break;
	case MG_WEBSOCKET_OPCODE_PONG:
		messageType = "pong";
		break;
	}
	printf("Websocket received %lu bytes of %s data from client %u\n",
	       (unsigned long)datasize,
	       messageType,
	       wsCliCtx->connectionNumber);

	return 1;
}


/* Handler indicating the connection to the client is closing. */
static void
ws_close_handler(const struct mg_connection *conn, void *user_data)
{
	(void)user_data; /* unused */

	/* Get websocket client context information. */
	struct tClientContext *wsCliCtx =
	    (struct tClientContext *)mg_get_user_connection_data(conn);

	/* DEBUG: Client has left. */
	printf("Client %u closing connection\n", wsCliCtx->connectionNumber);

	/* Free memory allocated for client context in ws_connect_handler() call. */
	free(wsCliCtx);
}


int
main(int argc, char *argv[])
{
	/* Initialize CivetWeb library without OpenSSL/TLS support. */
	mg_init_library(0);

	/* Start the server using the advanced API. */
	struct mg_callbacks callbacks = {0};
	void *user_data = NULL;

	struct mg_init_data mg_start_init_data = {0};
	mg_start_init_data.callbacks = &callbacks;
	mg_start_init_data.user_data = user_data;
	mg_start_init_data.configuration_options = SERVER_OPTIONS;

	struct mg_error_data mg_start_error_data = {0};
	char errtxtbuf[256] = {0};
	mg_start_error_data.text = errtxtbuf;
	mg_start_error_data.text_buffer_size = sizeof(errtxtbuf);

	struct mg_context *ctx =
	    mg_start2(&mg_start_init_data, &mg_start_error_data);
	if (!ctx) {
		fprintf(stderr, "Cannot start server: %s\n", errtxtbuf);
		mg_exit_library();
		return 1;
	}

	/* Register the websocket callback functions. */
	mg_set_websocket_handler_with_subprotocols(ctx,
	                                           WS_URL,
	                                           &wsprot,
	                                           ws_connect_handler,
	                                           ws_ready_handler,
	                                           ws_data_handler,
	                                           ws_close_handler,
	                                           user_data);

	/* Let the server run. */
	printf("Websocket server running\n");
	while (!g_exit) {
		sleep(1);
	}
	printf("Websocket server stopping\n");

	/* Stop server, disconnect all clients. Then deinitialize CivetWeb library.
	 */
	mg_stop(ctx);
	mg_exit_library();
}
