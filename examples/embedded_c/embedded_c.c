/*
* Copyright (c) 2013-2015 the CivetWeb developers
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
#define EXAMPLE_URI "/example"
#define EXIT_URI "/exit"
int exitNow = 0;


int ExampleHandler(struct mg_connection *conn, void *cbdata)
{
    mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
    mg_printf(conn, "<html><body>");
    mg_printf(conn, "<h2>This is an example text from a C handler</h2>");
    mg_printf(conn, "<p>To see a page from the A handler <a href=\"A\">click here</a></p>");
    mg_printf(conn, "<p>To see a page from the A/B handler <a href=\"A/B\">click here</a></p>");
    mg_printf(conn, "<p>To see a page from the *.foo handler <a href=\"xy.foo\">click here</a></p>");
    mg_printf(conn, "<p>To test websocket handler <a href=\"/websocket\">click here</a></p>");
    mg_printf(conn, "<p>To exit <a href=\"%s\">click here</a></p>",
              EXIT_URI);
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


int FooHandler(struct mg_connection *conn, void *cbdata)
{
    /* Handler may access the request info using mg_get_request_info */
    struct mg_request_info * req_info = mg_get_request_info(conn);

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
    "  connection = new WebSocket(wsproto + '//' + window.location.host + '/websocket.lua');\n"
    "  websock_text_field = document.getElementById('websock_text_field');\n"
    "  connection.onmessage = function (e) {\n"
    "    websock_text_field.innerHtml=e.data;\n"
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
int WebSocketConnectHandler(const struct mg_connection * conn, void *cbdata)
{
    return 0;
}

void WebSocketReadyHandler(const struct mg_connection * conn, void *cbdata)
{
    const char * text = "Hello from the websocket ready handler";
    mg_websocket_write(conn, WEBSOCKET_OPCODE_TEXT, text, strlen(text));
}

int WebsocketDataHandler(const struct mg_connection * conn, int bits, char * data, size_t len, void *cbdata)
{
    return 1;
}

void WebSocketCloseHandler(const struct mg_connection * conn, void *cbdata)
{
}

#endif


int main(int argc, char *argv[])
{
    const char * options[] = { "document_root", DOCUMENT_ROOT,
                               "listening_ports", PORT, 0
                             };
    struct mg_callbacks callbacks;
    struct mg_context *ctx;

    memset(&callbacks, 0, sizeof(callbacks));
    ctx = mg_start(&callbacks, 0, options);

    mg_set_request_handler(ctx, EXAMPLE_URI, ExampleHandler, 0);
    mg_set_request_handler(ctx, EXIT_URI, ExitHandler, 0);
    mg_set_request_handler(ctx, "/a", AHandler, 0);
    mg_set_request_handler(ctx, "/a/b", ABHandler, 0);
    mg_set_request_handler(ctx, "**.foo$", FooHandler, 0);
    mg_set_request_handler(ctx, "/websocket", WebSocketStartHandler, 0);
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
    }

    mg_stop(ctx);
    printf("Bye!\n");

    return 0;
}
