
#ifndef WEBSOCKCALLBACKS_H_INCLUDED
#define WEBSOCKCALLBACKS_H_INCLUDED

#include "civetweb.h"

#ifdef __cplusplus
extern "C" {
#endif

void websock_init_lib(struct mg_context *ctx);
void websock_exit_lib(struct mg_context *ctx);

void websock_send_broadcast(struct mg_context *ctx, const char * data, int data_len);

void websocket_ready_handler(struct mg_connection *conn);
int websocket_data_handler(struct mg_connection *conn, int flags, char *data, size_t data_len);
void connection_close_handler(struct mg_connection *conn);


#ifdef __cplusplus
}
#endif

#endif