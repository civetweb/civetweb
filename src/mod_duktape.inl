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

static const char *civetweb_conn_id = "\xFF"
                                      "civetweb_conn";

static void *
mg_duk_mem_alloc(void *udata, duk_size_t size)
{
	return mg_malloc(size);
}

static void *
mg_duk_mem_realloc(void *udata, void *ptr, duk_size_t newsize)
{
	return mg_realloc(ptr, newsize);
}

static void
mg_duk_mem_free(void *udata, void *ptr)
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

static duk_ret_t
duk_itf_write(duk_context *ctx)
{
	struct mg_connection *conn;
	duk_double_t ret;
	duk_size_t len = 0;
	const char *val = duk_require_lstring(ctx, -1, &len);

	/*
	    duk_push_global_stash(ctx);
	    duk_get_prop_string(ctx, -1, civetweb_conn_id);
	    conn = (struct mg_connection *)duk_to_pointer(ctx, -1);
	*/
	duk_push_current_function(ctx);
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

static duk_ret_t
duk_itf_read(duk_context *ctx)
{
	struct mg_connection *conn;
	char buf[1024];
	int len;

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

	len = mg_read(conn, buf, sizeof(buf));

	duk_push_lstring(ctx, buf, len);
	return 1;
}

static void
mg_exec_duktape_script(struct mg_connection *conn, const char *path)
{
	duk_context *ctx = NULL;

	conn->must_close = 1;

	/* Create Duktape interpreter state */
	ctx = duk_create_heap(mg_duk_mem_alloc,
	                      mg_duk_mem_realloc,
	                      mg_duk_mem_free,
	                      NULL,
	                      mg_duk_fatal_handler);
	if (!ctx) {
		mg_cry(conn, "Failed to create a Duktape heap.");
		goto exec_duktape_finished;
	}

	/* Add "conn" object */
	duk_push_global_object(ctx);
	duk_push_object(ctx); /* create a new table/object ("conn") */

	duk_push_c_function(ctx, duk_itf_write, 1 /* 1 = nargs */);
	duk_push_pointer(ctx, (void *)conn);
	duk_put_prop_string(ctx, -2, civetweb_conn_id);
	duk_put_prop_string(ctx, -2, "write"); /* add function conn.write */

	duk_push_c_function(ctx, duk_itf_read, 0 /* 0 = nargs */);
	duk_push_pointer(ctx, (void *)conn);
	duk_put_prop_string(ctx, -2, civetweb_conn_id);
	duk_put_prop_string(ctx, -2, "read"); /* add function conn.read */

	duk_push_string(ctx, conn->request_info.request_method);
	duk_put_prop_string(ctx, -2, "request_method"); /* add string conn.r... */

	duk_push_string(ctx, conn->request_info.request_uri);
	duk_put_prop_string(ctx, -2, "request_uri");

	duk_push_string(ctx, conn->request_info.local_uri);
	duk_put_prop_string(ctx, -2, "uri");

	duk_push_string(ctx, conn->request_info.http_version);
	duk_put_prop_string(ctx, -2, "http_version");

	duk_push_string(ctx, conn->request_info.query_string);
	duk_put_prop_string(ctx, -2, "query_string");

	duk_push_string(ctx, conn->request_info.remote_addr);
	duk_put_prop_string(ctx, -2, "remote_addr");

	duk_push_int(ctx, conn->request_info.remote_port);
	duk_put_prop_string(ctx, -2, "remote_port");

	duk_push_int(ctx, ntohs(conn->client.lsa.sin.sin_port));
	duk_put_prop_string(ctx, -2, "server_port");

	duk_put_prop_string(ctx, -2, "conn"); /* call the table "conn" */

	/* Add "civetweb" object */
	duk_push_global_object(ctx);
	duk_push_object(ctx); /* create a new table/object ("conn") */

	duk_push_string(ctx, CIVETWEB_VERSION);
	duk_put_prop_string(ctx, -2, "version");

	duk_push_string(ctx, script_name);
	duk_put_prop_string(ctx, -2, "script_name");

	/* TODO: Port the following Lua code - or alternatively add a function to
access all config params

	if (ctx != NULL) {
	    reg_string(L, "document_root", ctx->config[DOCUMENT_ROOT]);
	    reg_string(L, "auth_domain", ctx->config[AUTHENTICATION_DOMAIN]);
#if defined(USE_WEBSOCKET)
	    reg_string(L, "websocket_root", ctx->config[WEBSOCKET_ROOT]);
#endif

	    if (ctx->systemName != NULL) {
	        reg_string(L, "system", ctx->systemName);
	    }
	}
	*/

	duk_put_prop_string(ctx, -2, "civetweb"); /* call the table "civetweb" */

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
