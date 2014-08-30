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

const char * websocket_welcome_msg = "websocket welcome\n";
const size_t websocket_welcome_msg_len = 18 /* strlen(websocket_welcome_msg) */ ;
const char * websocket_acknowledge_msg = "websocket msg ok\n";
const size_t websocket_acknowledge_msg_len = 17 /* strlen(websocket_acknowledge_msg) */ ;
const char * websocket_goodbye_msg = "websocket bye\n";
const size_t websocket_goodbye_msg_len = 14 /* strlen(websocket_goodbye_msg) */ ;


/*************************************************************************************/
/* WEBSOCKET SERVER                                                                  */
/*************************************************************************************/
int websock_server_connect(const struct mg_connection * conn)
{
    printf("Server: Websocket connected\n");
    return 0; /* return 0 to accept every connection */
}

void websocket_server_ready(struct mg_connection * conn)
{
    printf("Server: Websocket ready\n");

    /* Send websocket welcome message */
    mg_lock_connection(conn);
    mg_websocket_write(conn, WEBSOCKET_OPCODE_TEXT, websocket_welcome_msg, websocket_welcome_msg_len);
    mg_unlock_connection(conn);
}

int websocket_server_data(struct mg_connection * conn, int bits, char *data, size_t data_len)
{
    printf("Server: Got %u bytes from the client\n", data_len);

    if (data_len<3 || 0!=memcmp(data, "bye", 3)) {
        /* Send websocket acknowledge message */
        mg_lock_connection(conn);
        mg_websocket_write(conn, WEBSOCKET_OPCODE_TEXT, websocket_acknowledge_msg, websocket_acknowledge_msg_len);
        mg_unlock_connection(conn);
    } else {
        /* Send websocket acknowledge message */
        mg_lock_connection(conn);
        mg_websocket_write(conn, WEBSOCKET_OPCODE_TEXT, websocket_goodbye_msg, websocket_goodbye_msg_len);
        mg_unlock_connection(conn);
    }

    return 1; /* return 1 to keep the connetion open */
}

void websocket_server_connection_close(struct mg_connection * conn)
{
    printf("Server: Close connection\n");

    /* Can not send a websocket goodbye message here - the connection is already closed */
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


/*************************************************************************************/
/* WEBSOCKET CLIENT                                                                  */
/*************************************************************************************/
struct {
    void * data;
    size_t len;
} client_data;

static int websocket_client_data_handler(struct mg_connection *conn, int flags, char *data, size_t data_len)
{
    printf("From server: ");
    fwrite(data, 1, data_len, stdout);
    printf("\n");

    /* TODO: extra arg (instead of global client_data) */
    client_data.data = malloc(data_len);
    assert(client_data.data != NULL);
    memcpy(client_data.data, data, data_len);
    client_data.len = data_len;

    return 1;
}


int main(int argc, char *argv[])
{
    struct mg_context *ctx;
    struct mg_connection* newconn;
    char ebuf[100];

    assert(websocket_welcome_msg_len == strlen(websocket_welcome_msg));

    /* First set up a websocket server */
    ctx = start_websocket_server();
    assert(ctx != NULL);

    /* Then connect a client */
    newconn = mg_websocket_client_connect("localhost", atoi(PORT), 0,
        ebuf, sizeof(ebuf),
        "/websocket", NULL, websocket_client_data_handler /* TODO: extra arg (instead of global client_data) */);

    if (newconn == NULL)
    {
        printf("Error: %s", ebuf);
        return 1;
    }

    sleep(1); /* Should get the websocket welcome message */
    assert(client_data.data != NULL);
    assert(client_data.len == websocket_welcome_msg_len);
    assert(!memcmp(client_data.data, websocket_welcome_msg, websocket_welcome_msg_len));
    free(client_data.data);
    client_data.data = NULL;
    client_data.len = 0;

    mg_websocket_write(newconn, WEBSOCKET_OPCODE_TEXT, "data1", 5);

    sleep(1); /* Should get the acknowledge message */
    assert(client_data.data != NULL);
    assert(client_data.len == websocket_acknowledge_msg_len);
    assert(!memcmp(client_data.data, websocket_acknowledge_msg, websocket_acknowledge_msg_len));
    free(client_data.data);
    client_data.data = NULL;
    client_data.len = 0;

    mg_websocket_write(newconn, WEBSOCKET_OPCODE_TEXT, "data2", 5);

    sleep(1); /* Should get the acknowledge message */
    assert(client_data.data != NULL);
    assert(client_data.len == websocket_acknowledge_msg_len);
    assert(!memcmp(client_data.data, websocket_acknowledge_msg, websocket_acknowledge_msg_len));
    free(client_data.data);
    client_data.data = NULL;
    client_data.len = 0;

    mg_websocket_write(newconn, WEBSOCKET_OPCODE_TEXT, "bye", 3);

    sleep(1); /* Should get the goodbye message */
    assert(client_data.data != NULL);
    assert(client_data.len == websocket_goodbye_msg_len);
    assert(!memcmp(client_data.data, websocket_goodbye_msg, websocket_goodbye_msg_len));
    free(client_data.data);
    client_data.data = NULL;
    client_data.len = 0;

    mg_close_connection(newconn);

    sleep(1); /* Won't get any message */

    mg_stop(ctx);
    printf("Server shutdown\n");

    sleep(10);

    return 0;
}
