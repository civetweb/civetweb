/* This file is part of the CivetWeb web server.
 * See https://github.com/civetweb/civetweb/
 * (C) 2015 by the CivetWeb authors, MIT license.
 */

#include "duktape.h"

/* TODO: This stub is currently not useful, since there is no way to communicate
 * with the client. */
static void mg_exec_lua_script(struct mg_connection *conn, const char *path)
{
	duk_context *ctx = NULL;

#ifdef WIN32
	(void)MakeConsole();
#endif

	ctx = duk_create_heap_default();
	if (!ctx) {
		fprintf(stderr, "Failed to create a Duktape heap.\n");
		goto finished;
	}

	if (duk_peval_file(ctx, path) != 0) {
		fprintf(stderr, "%s\n", duk_safe_to_string(ctx, -1));
		goto finished;
	}
	duk_pop(ctx); /* ignore result */

finished:
	duk_destroy_heap(ctx);
}
