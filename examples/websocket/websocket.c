#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "civetweb.h"
#include "WebSockCallbacks.h"

int main(void)
{
    struct mg_context *ctx = 0;
    struct mg_callbacks callback_funcs = {0};
    char inbuf[4];

    const char *server_options[] = {
        /* document_root: The path to the test function websock.htm */
        "document_root",     "../../examples/websocket",

        /* port: use http standard to match websocket url in websock.htm: ws://127.0.0.1/MyWebSock  */
        /*       if the port is changed here, it needs to be changed in websock.htm as well         */
        "listening_ports",   "80",

        NULL
    };

    websock_init_lib();

    callback_funcs.websocket_ready = websocket_ready_handler;
    callback_funcs.websocket_data = websocket_data_handler;
    callback_funcs.connection_close = connection_close_handler;
    ctx = mg_start(&callback_funcs, NULL, server_options);

    puts("Enter an (ASCII) character or * to exit:");
    for (;;) {
        fgets(inbuf, sizeof(inbuf), stdin);

        if (inbuf[0]=='*') {
           break;
        }
        inbuf[0] = toupper(inbuf[0]);
        websock_send_broadcast(inbuf, 1);
    }

    mg_stop(ctx);
    websock_exit_lib();

    return 0;
}
