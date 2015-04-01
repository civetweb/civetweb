//Copyright (c) 2013-2015 the Civetweb developers
//This example demonstrates the use of the response functions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "civetweb.h"

// This function will be called by civetweb on every new request.
static int begin_request_handler(struct mg_connection *conn)
{
    const struct mg_request_info *request_info = mg_get_request_info(conn);
    char content[100];

    // Prepare the message we're going to send
    int content_length = snprintf(content, sizeof(content),
                                  "Hello from civetweb! Remote port: %d\r\n",
                                  request_info->remote_port);

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%d", content_length);

    // Send HTTP reply to the client
    const char *header_data[] = {
        "Content-Type", "text/plain",
        "Content-Length", buffer,
        NULL
    };

    struct mg_response_header *headers = mg_construct_response_headers(header_data);

    // Construct a response message
    size_t len = 0;
    void *response = mg_construct_response_msg(conn, "1.1", 200, "OK", headers, content, content_length, &len);

    // free memory allocated by headers
    mg_free_response_headers(&headers);

    // send the response
    mg_write(conn, response, len);

    // free the response buffer
    mg_free_response_msg(&response);

    // Returning non-zero tells civetweb that our function has replied to
    // the client, and civetweb should not send client any more data.
    return 1;
}

int main(void)
{
    struct mg_context *ctx;
    struct mg_callbacks callbacks;

    // List of options. Last element must be NULL.
    const char *options[] = {"listening_ports", "8080", NULL};

    // Prepare callbacks structure. We have only one callback, the rest are NULL.
    memset(&callbacks, 0, sizeof(callbacks));
    callbacks.begin_request = begin_request_handler;

    // Start the web server.
    ctx = mg_start(&callbacks, NULL, options);

    // Wait until user hits "enter". Server is running in separate thread.
    // Navigating to http://localhost:8080 will invoke begin_request_handler().
    getchar();

    // Stop the server.
    mg_stop(ctx);

    return 0;
}
