/*
* Copyright (c) 2014 the Civetweb developers
* Copyright (c) 2014 Jordan Shelley
* https://github.com/jshelley
* License http://opensource.org/licenses/mit-license.php MIT License
*/

// Simple example program on how to use websocket client embedded C interface.
#ifdef _WIN32
#include <Windows.h>
#define sleep(x) Sleep(1000*(x))
#else
#include <unistd.h>
#endif

#include <assert.h>
#include <string.h>
#include "civetweb.h"

#define DOCUMENT_ROOT "."
#define PORT "8888"
#define SSL_CERT "./ssl/server.pem"


int websock_server_connect(const struct mg_connection * conn)
{
    printf("Server: Websocket connected\n");
    return 0; /* return 0 to accept every connection */
}

void websocket_server_ready(struct mg_connection * conn)
{
    printf("Server: Websocket ready\n");
}

int websocket_server_data(struct mg_connection * conn, int bits, char *data, size_t data_len)
{
    printf("Server: Got %u bytes from the client\n", data_len);
    return 1; /* return 1 to keep the connetion open */
}

void websocket_server_connection_close(struct mg_connection * conn)
{
    printf("Server: Close connection\n");
}

struct mg_context * start_websocket_server()
{
    const char * options[] = { "document_root", DOCUMENT_ROOT,
                               "ssl_certificate", SSL_CERT,
                               "listening_ports", PORT, 0
                             };
    struct mg_callbacks callbacks;
    struct mg_context *ctx;

    memset(&callbacks, 0, sizeof(callbacks));
    callbacks.websocket_connect = websock_server_connect;
    callbacks.websocket_ready = websocket_server_ready;
    callbacks.websocket_data = websocket_server_data;
    callbacks.connection_close = websocket_server_connection_close;
    ctx = mg_start(&callbacks, 0, options);

    return ctx;
}


static int websocket_client_data_handler(struct mg_connection *conn, int flags, char *data, size_t data_len)
{
    printf("From server: ");
    fwrite(data, 1, data_len, stdout);
    printf("\n");

    return 1;
}


int main(int argc, char *argv[])
{
    struct mg_context *ctx;
    struct mg_connection* newconn;
    char ebuf[100];

    /* First set up a websocket server */
    ctx = start_websocket_server();
    assert(ctx != NULL);

    newconn = mg_websocket_client_connect("localhost", atoi(PORT), 0,
                             ebuf, sizeof(ebuf),
                             "/websocket", NULL, websocket_client_data_handler);

    if (newconn == NULL)
    {
        printf("Error: %s", ebuf);
        return 1;
    }

    mg_websocket_write(newconn, WEBSOCKET_OPCODE_TEXT, "data1", 5);

    sleep(1);

    mg_websocket_write(newconn, WEBSOCKET_OPCODE_TEXT, "data2", 5);

    sleep(1);

    mg_websocket_write(newconn, WEBSOCKET_OPCODE_TEXT, "data3", 5);

    sleep(1);

    mg_websocket_write(newconn, WEBSOCKET_OPCODE_TEXT, "data4", 5);

    sleep(1);

    mg_close_connection(newconn);

    mg_stop(ctx);

    printf("Bye!\n");

    return 0;
}
