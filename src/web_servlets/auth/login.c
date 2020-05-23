#include "auth.h"

int serve_login(struct mg_connection *conn, void *cbdata)
{
	mg_printf(conn, "Hello serve login\n");
	return 1;
}