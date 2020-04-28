/* Stub for HTTP/2 */
static void process_new_http2_connection(struct mg_connection *conn)
{
	mg_send_http_error(conn, 505, "%s", "HTTP/2 not supported");
}
