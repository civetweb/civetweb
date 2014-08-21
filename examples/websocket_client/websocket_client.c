/*
* Copyright (c) 2013 No Face Press, LLC
* License http://opensource.org/licenses/mit-license.php MIT License
*/

// Simple example program on how to use Embedded C interface.
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <string.h>
#include "civetweb.h"

#define DOCUMENT_ROOT "."
#define PORT "8888"
#define SSL_CERT "./ssl/server.pem"

int websocket_data_handler(struct mg_connection *conn, int flags, char *data, size_t data_len)
{
    printf("From server: %s\r\n", data);

    return 1;
}

int main(int argc, char *argv[])
{

    const char * options[] = { "document_root", DOCUMENT_ROOT,
                               "ssl_certificate", SSL_CERT,
                               "listening_ports", PORT, 0
                             };
    struct mg_callbacks callbacks;
    struct mg_context *ctx;

    memset(&callbacks, 0, sizeof(callbacks));
    ctx = mg_start(&callbacks, 0, options);

    char ebuf[100];
    struct mg_connection* newconn = mg_client_websocket_connect("echo.websocket.org", 443, 1,
                             ebuf, sizeof(ebuf),
                             "/", "http://websocket.org",websocket_data_handler);

    if(newconn == NULL)
    {
        printf("Error: %s", ebuf);
        return 1;
    }

    mg_websocket_write(newconn, WEBSOCKET_OPCODE_TEXT, "data1", 5);

    sleep(5);

    mg_websocket_write(newconn, WEBSOCKET_OPCODE_TEXT, "data2", 5);

    sleep(5);

    mg_websocket_write(newconn, WEBSOCKET_OPCODE_TEXT, "data3", 5);

    sleep(5);

    mg_websocket_write(newconn, WEBSOCKET_OPCODE_TEXT, "data4", 5);

    sleep(5);

    mg_close_connection(newconn);

    printf("Bye!\n");

    return 0;
}
