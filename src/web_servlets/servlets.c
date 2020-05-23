#include <string.h>

#include "servlets.h"
#include "kitasuna/include/ymap.h"
#include "auth/auth.h"

static struct YMap AshibaServlets;

static int compare_ashiba_url(struct YPair *a, struct YPair *b)
{
	if (a == NULL && b == NULL) {
		return 0;
	} else if (a == NULL && b != NULL) {
		return -1;
	} else if (a != NULL && b == NULL) {
		return 1;
	} else {
		char *sa = (char *)YPairGetKey(a);
		char *sb = (char *)YPairGetKey(b);
		return strcmp(sa, sb);
	}
}

static void init_ashiba_servlets()
{
	YMapInitWithPairComparer(&AshibaServlets, compare_ashiba_url);
	
	YMapSetKeyValue(&AshibaServlets, "/auth/login", NULL, serve_login, NULL);
}

static void deinit_ashiba_servlets()
{
	YMapDestroy(&AshibaServlets);
}

static int ashiba_servlet(struct mg_connection *conn, void *cbdata)
{
	const struct mg_request_info *ri = mg_get_request_info(conn);
	if (ri == NULL) {
		return 0;
	}

	mg_request_handler h = (mg_request_handler)YMapGetKeyValue(&AshibaServlets, ri->request_uri);
	if (h != NULL) {
		return h(conn, cbdata);
	}

	return 0;
}


void register_ashiba_servlets(struct mg_context *ctx)
{
	if (ctx != NULL) {
		mg_set_request_handler(ctx, "/", ashiba_servlet, NULL);
	}

	init_ashiba_servlets();
}


void unregister_ashiba_servlets(struct mg_context *ctx)
{
	if (ctx != NULL) {
		mg_set_request_handler(ctx, "/", NULL, NULL);
	}

	deinit_ashiba_servlets();
}