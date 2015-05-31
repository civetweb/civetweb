/*
* Copyright (c) 2013-2015 the CivetWeb developers
* Copyright (c) 2013 No Face Press, LLC
* License http://opensource.org/licenses/mit-license.php MIT License
*/

/* Simple example program on how to use Embedded C interface. */
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <string.h>
#include "civetweb.h"


#define DOCUMENT_ROOT "."
#ifdef USE_IPV6
#define PORT "8888,[::]:8886"
#else
#define PORT "8888"
#endif
#define EXAMPLE_URI "/example"
#define EXIT_URI "/exit"
int exitNow = 0;


int ExampleHandler(struct mg_connection *conn, void *cbdata)
{
    mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
    mg_printf(conn, "<html><body>");
    mg_printf(conn, "<h2>This is an example text from a C handler</h2>");
    mg_printf(conn, "<p>To see a page from the A handler <a href=\"A\">click A</a></p>");
    mg_printf(conn, "<p>To see a page from the A handler <a href=\"A/A\">click A/A</a></p>");
    mg_printf(conn, "<p>To see a page from the A/B handler <a href=\"A/B\">click A/B</a></p>");
    mg_printf(conn, "<p>To see a page from the B handler (0) <a href=\"B\">click B</a></p>");
    mg_printf(conn, "<p>To see a page from the B handler (1) <a href=\"B/A\">click B/A</a></p>");
    mg_printf(conn, "<p>To see a page from the B handler (2) <a href=\"B/B\">click B/B</a></p>");
    mg_printf(conn, "<p>To see a page from the *.foo handler <a href=\"xy.foo\">click xy.foo</a></p>");
    mg_printf(conn, "<p>To test websocket handler <a href=\"/websocket\">click websocket</a></p>");
    mg_printf(conn, "<p>To exit <a href=\"%s\">click exit</a></p>", EXIT_URI);
    mg_printf(conn, "</body></html>\n");
    return 1;
}


int ExitHandler(struct mg_connection *conn, void *cbdata)
{
    mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n");
    mg_printf(conn, "Bye!\n");
    exitNow = 1;
    return 1;
}


int AHandler(struct mg_connection *conn, void *cbdata)
{
    mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
    mg_printf(conn, "<html><body>");
    mg_printf(conn, "<h2>This is the A handler!!!</h2>");
    mg_printf(conn, "</body></html>\n");
    return 1;
}


int ABHandler(struct mg_connection *conn, void *cbdata)
{
    mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
    mg_printf(conn, "<html><body>");
    mg_printf(conn, "<h2>This is the AB handler!!!</h2>");
    mg_printf(conn, "</body></html>\n");
    return 1;
}


int BXHandler(struct mg_connection *conn, void *cbdata)
{
    /* Handler may access the request info using mg_get_request_info */
    const struct mg_request_info * req_info = mg_get_request_info(conn);

    mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
    mg_printf(conn, "<html><body>");
    mg_printf(conn, "<h2>This is the BX handler %i!!!</h2>", (int)cbdata);
    mg_printf(conn, "<p>The actual uri is %s</p>", req_info->uri);
    mg_printf(conn, "</body></html>\n");
    return 1;
}


int FooHandler(struct mg_connection *conn, void *cbdata)
{
    /* Handler may access the request info using mg_get_request_info */
    const struct mg_request_info * req_info = mg_get_request_info(conn);

    mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
    mg_printf(conn, "<html><body>");
    mg_printf(conn, "<h2>This is the Foo handler!!!</h2>");
    mg_printf(conn, "<p>The request was:<br><pre>%s %s HTTP/%s</pre></p>",
              req_info->request_method, req_info->uri, req_info->http_version);
    mg_printf(conn, "</body></html>\n");
    return 1;
}


int WebSocketStartHandler(struct mg_connection *conn, void *cbdata)
{
    mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");

    mg_printf(conn, "<!DOCTYPE html>\n");
    mg_printf(conn, "<html>\n<head>\n");
    mg_printf(conn, "<meta charset=\"UTF-8\">\n");
    mg_printf(conn, "<title>Embedded websocket example</title>\n");
#ifdef USE_WEBSOCKET
    /* mg_printf(conn, "<script type=\"text/javascript\"><![CDATA[\n"); ... xhtml style */
    mg_printf(conn, "<script>\n");
    mg_printf(conn,
    "function load() {\n"
    "  var wsproto = (location.protocol === 'https:') ? 'wss:' : 'ws:';\n"
    "  connection = new WebSocket(wsproto + '//' + window.location.host + '/websocket');\n"
    "  websock_text_field = document.getElementById('websock_text_field');\n"
    "  connection.onmessage = function (e) {\n"
    "    websock_text_field.innerHTML=e.data;\n"
    "  }\n"
    "  connection.onerror = function (error) {\n"
    "    alert('WebSocket error');\n"
    "    connection.close();\n"
    "  }\n"
    "}\n"
    );
    /* mg_printf(conn, "]]></script>\n"); ... xhtml style */
    mg_printf(conn, "</script>\n");
    mg_printf(conn, "</head>\n<body onload=\"load()\">\n");
    mg_printf(conn, "<div id='websock_text_field'>No websocket connection yet</div>\n");
#else
    mg_printf(conn, "</head>\n<body>\n");
    mg_printf(conn, "Example not compiled with USE_WEBSOCKET\n");
#endif
    mg_printf(conn, "</body>\n</html>\n");
    return 1;
}


#ifdef USE_WEBSOCKET

#define MAX_WS_CLIENTS 5 /* just for the test: a small number that can be reached */
                         /* a real server should use a much higher number here */

struct t_ws_client {
    struct mg_connection * conn;
    int state;
} static ws_clients[MAX_WS_CLIENTS];

#define ASSERT(x) {if (!(x)) {fprintf(stderr, "Assertion failed in line %u\n", (unsigned)__LINE__);}}

static unsigned long cnt;

int WebSocketConnectHandler(const struct mg_connection * conn, void *cbdata)
{
    struct mg_context *ctx = mg_get_context(conn);
    int reject = 1;
    int i;

    mg_lock_context(ctx);
    for (i=0; i<MAX_WS_CLIENTS; i++) {
        if (ws_clients[i].conn == NULL) {
            ws_clients[i].conn = (struct mg_connection *) conn;
            ws_clients[i].state = 1;
            mg_set_user_connection_data(conn, (void*) (ws_clients+i));
            reject = 0;
            break;
        }
    }
    mg_unlock_context(ctx);

    fprintf(stdout, "Websocket client %s\r\n\r\n", (reject ? "rejected" : "accepted"));
    return reject;
}

void WebSocketReadyHandler(struct mg_connection * conn, void *cbdata)
{
    const char * text = "Hello from the websocket ready handler";
    struct t_ws_client * client = mg_get_user_connection_data(conn);

    mg_websocket_write(conn, WEBSOCKET_OPCODE_TEXT, text, strlen(text));
    fprintf(stdout, "Greeting message sent to websocket client\r\n\r\n");
    ASSERT(client->conn == conn);
    ASSERT(client->state == 1);

    client->state = 2;
}

int WebsocketDataHandler(struct mg_connection * conn, int bits, char * data, size_t len, void *cbdata)
{
    struct t_ws_client * client = mg_get_user_connection_data(conn);
    ASSERT(client->conn == conn);
    ASSERT(client->state >= 1);

    fprintf(stdout, "Websocket got data:\r\n");
    fwrite(data, len, 1, stdout);
    fprintf(stdout, "\r\n\r\n");

    return 1;
}

void WebSocketCloseHandler(const struct mg_connection * conn, void *cbdata)
{
    struct mg_context *ctx = mg_get_context(conn);
    struct t_ws_client * client = mg_get_user_connection_data(conn);
    ASSERT(client->conn == conn);
    ASSERT(client->state >= 1);

    mg_lock_context(ctx);
    client->state = 0;
    client->conn = NULL;
    mg_unlock_context(ctx);

    fprintf(stdout, "Client droped from the set of webserver connections\r\n\r\n");
}

void InformWebsockets(struct mg_context *ctx)
{
    char text[32];
    int i;

    sprintf(text, "%lu", ++cnt);

    mg_lock_context(ctx);
    for (i=0; i<MAX_WS_CLIENTS; i++) {
        if (ws_clients[i].state == 2) {
            mg_websocket_write(ws_clients[i].conn, WEBSOCKET_OPCODE_TEXT, text, strlen(text));
        }
    }
    mg_unlock_context(ctx);
}
#endif


int main(int argc, char *argv[])
{
    const char * options[] = { "document_root", DOCUMENT_ROOT,
                               "listening_ports", PORT,
                               "request_timeout_ms", "10000",
#ifdef USE_WEBSOCKET
                               "websocket_timeout_ms", "3600000",
#endif
                               0
                             };
    struct mg_callbacks callbacks;
    struct mg_context *ctx;

    memset(&callbacks, 0, sizeof(callbacks));
    ctx = mg_start(&callbacks, 0, options);

    /* Handler EXAMPLE_URI to explain the example */
    mg_set_request_handler(ctx, EXAMPLE_URI, ExampleHandler, 0);
    mg_set_request_handler(ctx, EXIT_URI, ExitHandler, 0);

    /* Handler for /A* and special handler for /A/B */
    mg_set_request_handler(ctx, "/A", AHandler, 0);
    mg_set_request_handler(ctx, "/A/B", ABHandler, 0);

    /* Handler for /B, /B/A, /B/B but not for /B* */
    mg_set_request_handler(ctx, "/B$", BXHandler, (void*)0);
    mg_set_request_handler(ctx, "/B/A$", BXHandler, (void*)1);
    mg_set_request_handler(ctx, "/B/B$", BXHandler, (void*)2);

    /* Handler for all files with .foo extention */
    mg_set_request_handler(ctx, "**.foo$", FooHandler, 0);

    /* HTTP site to open a websocket connection */
    mg_set_request_handler(ctx, "/websocket", WebSocketStartHandler, 0);

    /* WS site for the websocket connection */
    mg_set_websocket_handler(ctx, "/websocket", WebSocketConnectHandler, WebSocketReadyHandler, WebsocketDataHandler, WebSocketCloseHandler, 0);

    printf("Browse files at http://localhost:%s/\n", PORT);
    printf("Run example at http://localhost:%s%s\n", PORT, EXAMPLE_URI);
    printf("Exit at http://localhost:%s%s\n", PORT, EXIT_URI);

    while (!exitNow) {
#ifdef _WIN32
        Sleep(1000);
#else
        sleep(1);
#endif
#ifdef USE_WEBSOCKET
        InformWebsockets(ctx);
#endif
    }

    mg_stop(ctx);
    printf("Bye!\n");

    return 0;
}
