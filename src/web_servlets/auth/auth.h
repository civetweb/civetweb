#ifndef _AUTH_H
#define _AUTH_H

#include "civetweb.h"

#ifdef __cplusplus
extern "C" {
#endif

int serve_login(struct mg_connection *conn, void *cbdata);

#ifdef __cplusplus
}
#endif

#endif