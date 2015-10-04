/* This file is part of the CivetWeb web server.
 * See https://github.com/civetweb/civetweb/
 * (C) 2015 by the CivetWeb authors, MIT license.
 */

#include "duktape.h"

/* TODO: the mg context should be added to duktape as well */
/* Alternative: redefine a new, clean API from scratch (instead of using mg),
 * or at least do not add problematic functions. */
/* For evaluation purposes, currently only "send" is supported.
 * All other ~50 functions will be added later. */

/* Note: This is only experimental support, so any API may still change. */


static const char *civetweb_conn_id = "\xFF" "civetweb_conn";


static void *mg_duk_mem_alloc(void *udata, duk_size_t size)
{
	return mg_malloc(size);
}


static void *mg_duk_mem_realloc(void *udata, void *ptr, duk_size_t newsize)
{
	return mg_realloc(ptr, newsize);
}


static void mg_duk_mem_free(void *udata, void *ptr)
{
	mg_free(ptr);
}


static void
mg_duk_fatal_handler(duk_context *ctx, duk_errcode_t code, const char *msg)
{
	/* TODO: check if this handler is required - duk_peval_file probably
	 * already avoids that this function is called. */
	/* TODO: test this handler (if it is called) */
	struct mg_connection *conn;

	duk_push_global_stash(ctx);
	duk_get_prop_string(ctx, -1, civetweb_conn_id);
	conn = (struct mg_connection *)duk_to_pointer(ctx, -1);

	mg_cry(conn, "%s", msg);
}


static duk_ret_t duk_itf_send(duk_context *ctx)
{
	struct mg_connection *conn;
	duk_double_t ret;
	duk_size_t len = 0;
	const char *val = duk_require_lstring(ctx, -1, &len);

	duk_push_global_stash(ctx);
	duk_get_prop_string(ctx, -1, civetweb_conn_id);
	conn = (struct mg_connection *)duk_to_pointer(ctx, -1);

	if (!conn) {
		duk_error(ctx,
		          DUK_ERR_INTERNAL_ERROR,
		          "function not available without connection object");
		/* probably never reached, but satisfies static code analysis */
		return DUK_RET_INTERNAL_ERROR;
	}

	ret = mg_write(conn, val, len);

	duk_push_number(ctx, ret);
	return 1;
}


static void mg_exec_duktape_script(struct mg_connection *conn, const char *path)
{
	duk_context *ctx = NULL;

	conn->must_close = 1;

	ctx = duk_create_heap(mg_duk_mem_alloc,
	                      mg_duk_mem_realloc,
	                      mg_duk_mem_free,
	                      NULL,
	                      mg_duk_fatal_handler);
	if (!ctx) {
		mg_cry(conn, "Failed to create a Duktape heap.");
		goto exec_duktape_finished;
	}

	duk_push_global_object(ctx);
	duk_push_c_function(ctx, duk_itf_send, 1 /*nargs*/);
	duk_put_prop_string(ctx, -2, "send");

	duk_push_global_stash(ctx);
	duk_push_pointer(ctx, (void *)conn);
	duk_put_prop_string(ctx, -2, civetweb_conn_id);

	if (duk_peval_file(ctx, path) != 0) {
		mg_cry(conn, "%s", duk_safe_to_string(ctx, -1));
		goto exec_duktape_finished;
	}
	duk_pop(ctx); /* ignore result */

exec_duktape_finished:
	duk_destroy_heap(ctx);
}
