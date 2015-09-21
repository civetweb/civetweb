/* This file is part of the CivetWeb web server.
 * See https://github.com/civetweb/civetweb/
 * (C) 2015 by the CivetWeb authors, MIT license.
 */

#include "duktape.h"

/* TODO: Malloc function should use mg_malloc/mg_free */

/* TODO: the mg context should be added to duktape as well */
/* Alternative: redefine a new, clean API from scratch (instead of using mg),
 * or at least do not add problematic functions. */
/* Note: This is only experimental support, so any API may still change. */


/* TODO: use upvalues (or whatever equivalent) */
static struct mg_connection *xxx_conn = NULL;

static duk_ret_t duk_itf_send(duk_context *ctx)
{
	duk_size_t len = 0;
	const char *val = duk_require_lstring(ctx, -1, &len);

	mg_write(xxx_conn, val, len);

	duk_push_true(ctx);
	return 1;
}


static void mg_exec_duktape_script(struct mg_connection *conn, const char *path)
{
	duk_context *ctx = NULL;

	conn->must_close = 1;

	ctx = duk_create_heap_default();
	if (!ctx) {
		mg_cry(conn, "Failed to create a Duktape heap.");
		goto exec_duktape_finished;
	}

	duk_push_global_object(ctx);
	duk_push_c_function(ctx, duk_itf_send, 1 /*nargs*/);
	duk_put_prop_string(ctx, -2, "send");

	xxx_conn = conn;

	if (duk_peval_file(ctx, path) != 0) {
		mg_cry(conn, "%s", duk_safe_to_string(ctx, -1));
		goto exec_duktape_finished;
	}
	duk_pop(ctx); /* ignore result */

exec_duktape_finished:
	duk_destroy_heap(ctx);
}
