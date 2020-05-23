#ifndef ASHIBA_SERVLETS_H
#define ASHIBA_SERVLETS_H

#include "civetweb.h"

#ifdef __cplusplus
extern "C" {
#endif

struct AshibaServlet {
	char *url;
	char *method;
	mg_request_handler servlet;
	void *data;
};


void register_ashiba_servlets(struct mg_context *ctx);
void unregister_ashiba_servlets(struct mg_context *ctx);


#ifdef __cplusplus
}
#endif
#endif