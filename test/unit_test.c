/* Copyright (c) 2013-2015 the Civetweb developers
 * Copyright (c) 2004-2013 Sergey Lyubka
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#if defined(_MSC_VER)
#pragma message("Warning: " __FILE__ " is obsolete. See note below.")
#else
#pragma message __FILE__ ": This file is obsolete. See note below."
#endif

/* Note: The unit_test.c file is mostly obsolete, since the current unit
 * tests are performed by the CMake build framework based on the
 * CMakeList (https://github.com/civetweb/civetweb/blob/master/test/CMakeLists.txt).
 * The tests do no longer use unit_test.c but public_func.c, public_server.c,
 * private.c, private_exe.c and others.
 * This file is left here for reference and will be removed in the future.
 * It is no longer actively maintained.
 */


/* Unit test for the civetweb web server. Tests embedded API.
 */
#define USE_WEBSOCKET

#ifndef _WIN32
#define __cdecl
#define USE_IPV6
#endif

/* USE_* definitions must be made before #include "civetweb.c" !
 * We include the source file so that our object file will have visibility to
 * all the static functions.
 */

#include "civetweb.c"

void check_func(int condition, const char *cond_txt, unsigned line);

static int s_total_tests = 0;
static int s_failed_tests = 0;

void check_func(int condition, const char *cond_txt, unsigned line)
{
	++s_total_tests;
	if (!condition) {
		printf("Fail on line %d: [%s]\n", line, cond_txt);
		++s_failed_tests;
	}
}

#define ASSERT(expr)                                                           \
	do {                                                                       \
		check_func(expr, #expr, __LINE__);                                     \
	} while (0)

#define REQUIRE(expr)                                                          \
	do {                                                                       \
		check_func(expr, #expr, __LINE__);                                     \
		if (!(expr)) {                                                         \
			exit(EXIT_FAILURE);                                                \
		}                                                                      \
	} while (0)

#define HTTP_PORT "8080"
#ifdef NO_SSL
#define HTTPS_PORT HTTP_PORT
#define LISTENING_ADDR "127.0.0.1:" HTTP_PORT
#else
#define HTTP_REDIRECT_PORT "8088"
#define HTTPS_PORT "8443"
#define LISTENING_ADDR                                                         \
	"127.0.0.1:" HTTP_PORT ",127.0.0.1:" HTTP_REDIRECT_PORT "r"                \
	",127.0.0.1:" HTTPS_PORT "s"
#endif

static void test_parse_http_message()
{
	struct mg_request_info ri;
	char req1[] = "GET / HTTP/1.1\r\n\r\n";
	char req2[] = "BLAH / HTTP/1.1\r\n\r\n";
	char req3[] = "GET / HTTP/1.1\r\nBah\r\n";
	char req4[] = "GET / HTTP/1.1\r\nA: foo bar\r\nB: bar\r\nbaz:\r\n\r\n";
	char req5[] = "GET / HTTP/1.1\r\n\r\n";
	char req6[] = "G";
	char req7[] = " blah ";
	char req8[] = " HTTP/1.1 200 OK \n\n";
	char req9[] = "HTTP/1.1 200 OK\r\nConnection: close\r\n\r\n";

	ASSERT(parse_http_message(req9, sizeof(req9), &ri) == sizeof(req9) - 1);
	ASSERT(ri.num_headers == 1);

	ASSERT(parse_http_message(req1, sizeof(req1), &ri) == sizeof(req1) - 1);
	ASSERT(strcmp(ri.http_version, "1.1") == 0);
	ASSERT(ri.num_headers == 0);

	ASSERT(parse_http_message(req2, sizeof(req2), &ri) == -1);
	ASSERT(parse_http_message(req3, sizeof(req3), &ri) == 0);
	ASSERT(parse_http_message(req6, sizeof(req6), &ri) == 0);
	ASSERT(parse_http_message(req7, sizeof(req7), &ri) == 0);
	ASSERT(parse_http_message("", 0, &ri) == 0);
	ASSERT(parse_http_message(req8, sizeof(req8), &ri) == sizeof(req8) - 1);

	/* TODO(lsm): Fix this. Header value may span multiple lines. */
	ASSERT(parse_http_message(req4, sizeof(req4), &ri) == sizeof(req4) - 1);
	ASSERT(strcmp(ri.http_version, "1.1") == 0);
	ASSERT(ri.num_headers == 3);
	ASSERT(strcmp(ri.http_headers[0].name, "A") == 0);
	ASSERT(strcmp(ri.http_headers[0].value, "foo bar") == 0);
	ASSERT(strcmp(ri.http_headers[1].name, "B") == 0);
	ASSERT(strcmp(ri.http_headers[1].value, "bar") == 0);
	ASSERT(strcmp(ri.http_headers[2].name, "baz") == 0);
	ASSERT(strcmp(ri.http_headers[2].value, "") == 0);

	ASSERT(parse_http_message(req5, sizeof(req5), &ri) == sizeof(req5) - 1);
	ASSERT(strcmp(ri.request_method, "GET") == 0);
	ASSERT(strcmp(ri.http_version, "1.1") == 0);
}

static void test_should_keep_alive(void)
{
	struct mg_connection conn;
	struct mg_context ctx;
	char req1[] = "GET / HTTP/1.1\r\n\r\n";
	char req2[] = "GET / HTTP/1.0\r\n\r\n";
	char req3[] = "GET / HTTP/1.1\r\nConnection: close\r\n\r\n";
	char req4[] = "GET / HTTP/1.1\r\nConnection: keep-alive\r\n\r\n";

	memset(&conn, 0, sizeof(conn));
	conn.ctx = &ctx;
	ASSERT(parse_http_message(req1, sizeof(req1), &conn.request_info) ==
	       sizeof(req1) - 1);

	ctx.config[ENABLE_KEEP_ALIVE] = "no";
	ASSERT(should_keep_alive(&conn) == 0);

	ctx.config[ENABLE_KEEP_ALIVE] = "yes";
	ASSERT(should_keep_alive(&conn) == 1);

	conn.must_close = 1;
	ASSERT(should_keep_alive(&conn) == 0);

	conn.must_close = 0;
	parse_http_message(req2, sizeof(req2), &conn.request_info);
	ASSERT(should_keep_alive(&conn) == 0);

	parse_http_message(req3, sizeof(req3), &conn.request_info);
	ASSERT(should_keep_alive(&conn) == 0);

	parse_http_message(req4, sizeof(req4), &conn.request_info);
	ASSERT(should_keep_alive(&conn) == 1);

	conn.status_code = 401;
	ASSERT(should_keep_alive(&conn) == 0);

	conn.status_code = 200;
	conn.must_close = 1;
	ASSERT(should_keep_alive(&conn) == 0);
}

static void test_match_prefix(void)
{
	ASSERT(match_prefix("/api", 4, "/api") == 4);
	ASSERT(match_prefix("/a/", 3, "/a/b/c") == 3);
	ASSERT(match_prefix("/a/", 3, "/ab/c") == -1);
	ASSERT(match_prefix("/*/", 3, "/ab/c") == 4);
	ASSERT(match_prefix("**", 2, "/a/b/c") == 6);
	ASSERT(match_prefix("/*", 2, "/a/b/c") == 2);
	ASSERT(match_prefix("*/*", 3, "/a/b/c") == 2);
	ASSERT(match_prefix("**/", 3, "/a/b/c") == 5);
	ASSERT(match_prefix("**.foo|**.bar", 13, "a.bar") == 5);
	ASSERT(match_prefix("a|b|cd", 6, "cdef") == 2);
	ASSERT(match_prefix("a|b|c?", 6, "cdef") == 2);
	ASSERT(match_prefix("a|?|cd", 6, "cdef") == 1);
	ASSERT(match_prefix("/a/**.cgi", 9, "/foo/bar/x.cgi") == -1);
	ASSERT(match_prefix("/a/**.cgi", 9, "/a/bar/x.cgi") == 12);
	ASSERT(match_prefix("**/", 3, "/a/b/c") == 5);
	ASSERT(match_prefix("**/$", 4, "/a/b/c") == -1);
	ASSERT(match_prefix("**/$", 4, "/a/b/") == 5);
	ASSERT(match_prefix("$", 1, "") == 0);
	ASSERT(match_prefix("$", 1, "x") == -1);
	ASSERT(match_prefix("*$", 2, "x") == 1);
	ASSERT(match_prefix("/$", 2, "/") == 1);
	ASSERT(match_prefix("**/$", 4, "/a/b/c") == -1);
	ASSERT(match_prefix("**/$", 4, "/a/b/") == 5);
	ASSERT(match_prefix("*", 1, "/hello/") == 0);
	ASSERT(match_prefix("**.a$|**.b$", 11, "/a/b.b/") == -1);
	ASSERT(match_prefix("**.a$|**.b$", 11, "/a/b.b") == 6);
	ASSERT(match_prefix("**.a$|**.b$", 11, "/a/B.A") == 6);
	ASSERT(match_prefix("**o$", 4, "HELLO") == 5);
}

static void test_remove_double_dots()
{
	struct {
		char before[20], after[20];
	} data[] = {
	    {"////a", "/a"},
	    {"/.....", "/."},
	    {"/......", "/"},
	    {"...", "..."},
	    {"/...///", "/./"},
	    {"/a...///", "/a.../"},
	    {"/.x", "/.x"},
	    {"/\\", "/"},
	    {"/a\\", "/a\\"},
	    {"/a\\\\...", "/a\\."},
	};
	size_t i;

	for (i = 0; i < ARRAY_SIZE(data); i++) {
		remove_double_dots_and_double_slashes(data[i].before);
		ASSERT(strcmp(data[i].before, data[i].after) == 0);
	}
}

static char *read_file(const char *path, int *size)
{
	FILE *fp;
	struct stat st;
	char *data = NULL;
	if ((fp = fopen(path, "rb")) != NULL && !fstat(fileno(fp), &st)) {
		*size = (int)st.st_size;
		data = mg_malloc(*size);
		ASSERT(data != NULL);
		ASSERT(fread(data, 1, *size, fp) == (size_t)*size);
		fclose(fp);
	}
	return data;
}

static long fetch_data_size = 1024 * 1024;
static char *fetch_data;
static const char *inmemory_file_data = "hi there";
static const char *upload_filename = "upload_test.txt";
#if 0
static const char *upload_filename2 = "upload_test2.txt";
#endif
static const char *upload_ok_message = "upload successful";

static const char *
open_file_cb(const struct mg_connection *conn, const char *path, size_t *size)
{
	(void)conn;
	if (!strcmp(path, "./blah")) {
		*size = strlen(inmemory_file_data);
		return inmemory_file_data;
	}
	return NULL;
}

#if defined(MG_LEGACY_INTERFACE)
static void upload_cb(struct mg_connection *conn, const char *path)
{
	const struct mg_request_info *ri = mg_get_request_info(conn);
	char *p1, *p2;
	int len1, len2;

	if (atoi(ri->query_string) == 1) {
		ASSERT(!strcmp(path, "./upload_test.txt"));
		ASSERT((p1 = read_file("src/civetweb.c", &len1)) != NULL);
		ASSERT((p2 = read_file(path, &len2)) != NULL);
		ASSERT(len1 == len2);
		ASSERT(memcmp(p1, p2, len1) == 0);
		mg_free(p1);
		mg_free(p2);
		remove(upload_filename);
	} else if (atoi(ri->query_string) == 2) {
		if (!strcmp(path, "./upload_test.txt")) {
			ASSERT((p1 = read_file("include/civetweb.h", &len1)) != NULL);
			ASSERT((p2 = read_file(path, &len2)) != NULL);
			ASSERT(len1 == len2);
			ASSERT(memcmp(p1, p2, len1) == 0);
			mg_free(p1);
			mg_free(p2);
			remove(upload_filename);
		} else if (!strcmp(path, "./upload_test2.txt")) {
			ASSERT((p1 = read_file("README.md", &len1)) != NULL);
			ASSERT((p2 = read_file(path, &len2)) != NULL);
			ASSERT(len1 == len2);
			ASSERT(memcmp(p1, p2, len1) == 0);
			mg_free(p1);
			mg_free(p2);
			remove(upload_filename);
		} else {
			ASSERT(0);
		}
	} else {
		ASSERT(0);
	}

	mg_printf(conn,
	          "HTTP/1.0 200 OK\r\nContent-Length: %d\r\n\r\n%s",
	          (int)strlen(upload_ok_message),
	          upload_ok_message);
}
#endif

static int begin_request_handler_cb(struct mg_connection *conn)
{

	const struct mg_request_info *ri = mg_get_request_info(conn);
	int req_len = (int)(ri->content_length);
	const char *s_req_len = mg_get_header(conn, "Content-Length");
	char *data;
	long to_write, write_now;
	int bytes_read, bytes_written;

	ASSERT(((req_len == -1) && (s_req_len == NULL)) ||
	       ((s_req_len != NULL) && (req_len = atol(s_req_len))));

	if (!strncmp(ri->uri, "/data/", 6)) {
		if (!strcmp(ri->uri + 6, "all")) {
			to_write = fetch_data_size;
		} else {
			to_write = atol(ri->uri + 6);
		}
		mg_printf(conn,
		          "HTTP/1.1 200 OK\r\n"
		          "Connection: close\r\n"
		          "Content-Length: %li\r\n"
		          "Content-Type: text/plain\r\n\r\n",
		          to_write);
		while (to_write > 0) {
			write_now = to_write > fetch_data_size ? fetch_data_size : to_write;
			bytes_written = mg_write(conn, fetch_data, write_now);
			ASSERT(bytes_written == write_now);
			if (bytes_written < 0) {
				ASSERT(0);
				break;
			}
			to_write -= bytes_written;
		}
		close_connection(conn);
		return 1;
	}

	if (!strcmp(ri->uri, "/content_length")) {
		if (req_len > 0) {
			data = mg_malloc(req_len);
			assert(data != NULL);
			bytes_read = mg_read(conn, data, req_len);
			ASSERT(bytes_read == req_len);

			mg_printf(conn,
			          "HTTP/1.1 200 OK\r\n"
			          "Connection: close\r\n"
			          "Content-Length: %d\r\n" /* The official definition */
			          "Content-Type: text/plain\r\n\r\n",
			          bytes_read);
			mg_write(conn, data, bytes_read);

			mg_free(data);
		} else {
			data = mg_malloc(1024);
			assert(data != NULL);
			bytes_read = mg_read(conn, data, 1024);

			mg_printf(conn,
			          "HTTP/1.1 200 OK\r\n"
			          "Connection: close\r\n"
			          "Content-Type: text/plain\r\n\r\n");
			mg_write(conn, data, bytes_read);

			mg_free(data);
		}
		close_connection(conn);
		return 1;
	}

#if defined(MG_LEGACY_INTERFACE)
	if (!strcmp(ri->uri, "/upload")) {
		ASSERT(ri->query_string != NULL);
		ASSERT(mg_upload(conn, ".") == atoi(ri->query_string));
	}
#endif

	return 0;
}

static int log_message_cb(const struct mg_connection *conn, const char *msg)
{
	(void)conn;
	printf("%s\n", msg);
	return 0;
}


int (*begin_request)(struct mg_connection *);
void (*end_request)(const struct mg_connection *, int reply_status_code);
int (*log_message)(const struct mg_connection *, const char *message);
int (*init_ssl)(void *ssl_context, void *user_data);
int (*websocket_connect)(const struct mg_connection *);
void (*websocket_ready)(struct mg_connection *);
int (*websocket_data)(struct mg_connection *,
                      int bits,
                      char *data,
                      size_t data_len);
void (*connection_close)(struct mg_connection *);
const char *(*open_file)(const struct mg_connection *,
                         const char *path,
                         size_t *data_len);
void (*init_lua)(struct mg_connection *, void *lua_context);
void (*upload)(struct mg_connection *, const char *file_name);
int (*http_error)(struct mg_connection *, int status);

static struct mg_callbacks CALLBACKS;

static void init_CALLBACKS()
{
	memset(&CALLBACKS, 0, sizeof(CALLBACKS));
	CALLBACKS.begin_request = begin_request_handler_cb;
	CALLBACKS.log_message = log_message_cb;
	CALLBACKS.open_file = open_file_cb;
#if defined(MG_LEGACY_INTERFACE)
	CALLBACKS.upload = upload_cb;
#endif
};

static const char *OPTIONS[] = {
    "document_root",
    ".",
    "listening_ports",
    LISTENING_ADDR,
    "enable_keep_alive",
    "yes",
#ifndef NO_SSL
    "ssl_certificate",
    "../resources/ssl_cert.pem",
#endif
    NULL,
};

static char *read_conn(struct mg_connection *conn, int *size)
{
	char buf[100], *data = NULL;
	int len;
	*size = 0;
	while ((len = mg_read(conn, buf, sizeof(buf))) > 0) {
		*size += len;
		data = mg_realloc(data, *size);
		ASSERT(data != NULL);
		memcpy(data + *size - len, buf, len);
	}
	return data;
}

#ifdef MEMORY_DEBUGGING
extern unsigned long mg_memory_debug_blockCount;
extern unsigned long mg_memory_debug_totalMemUsed;
#endif

static void ut_mg_stop(struct mg_context *ctx)
{
	/* mg_stop for unit_test */
	mg_stop(ctx);
#ifdef MEMORY_DEBUGGING
	ASSERT(mg_memory_debug_blockCount == 0);
	ASSERT(mg_memory_debug_totalMemUsed == 0);
	mg_memory_debug_blockCount = 0;
	mg_memory_debug_totalMemUsed = 0;
#endif

	mg_sleep(31000); /* This is required to ensure the operating system already
	                    allows to use the port again */
}

static void test_mg_download(int use_ssl)
{

	const char *test_data = "123456789A123456789B";

	char *p1, *p2, ebuf[100];
	const char *h;
	int i, len1, len2, port;
	struct mg_connection *conn;
	struct mg_context *ctx;
	const struct mg_request_info *ri;

	if (use_ssl) {
		port = atoi(HTTPS_PORT);
	} else {
		port = atoi(HTTP_PORT);
	}

	ctx = mg_start(&CALLBACKS, NULL, OPTIONS);

	ASSERT(ctx != NULL);

	ASSERT(mg_download(NULL, port, use_ssl, ebuf, sizeof(ebuf), "%s", "") ==
	       NULL);
	ASSERT(mg_download("localhost", 0, use_ssl, ebuf, sizeof(ebuf), "%s", "") ==
	       NULL);
	ASSERT(
	    mg_download("localhost", port, use_ssl, ebuf, sizeof(ebuf), "%s", "") ==
	    NULL);

	/* Fetch nonexistent file, should see 404 */
	ASSERT((conn = mg_download("localhost",
	                           port,
	                           use_ssl,
	                           ebuf,
	                           sizeof(ebuf),
	                           "%s",
	                           "GET /gimbec HTTP/1.0\r\n\r\n")) != NULL);
	ASSERT(strcmp(conn->request_info.uri, "404") == 0);
	mg_close_connection(conn);

	if (use_ssl) {
		ASSERT((conn = mg_download("google.com",
		                           443,
		                           1,
		                           ebuf,
		                           sizeof(ebuf),
		                           "%s",
		                           "GET / HTTP/1.0\r\n\r\n")) != NULL);
		mg_close_connection(conn);
	} else {
		ASSERT((conn = mg_download("google.com",
		                           80,
		                           0,
		                           ebuf,
		                           sizeof(ebuf),
		                           "%s",
		                           "GET / HTTP/1.0\r\n\r\n")) != NULL);
		mg_close_connection(conn);
	}

	/* Fetch unit_test.c, should succeed */
	ASSERT((conn = mg_download("localhost",
	                           port,
	                           use_ssl,
	                           ebuf,
	                           sizeof(ebuf),
	                           "%s",
	                           "GET /unit_test.c HTTP/1.0\r\n\r\n")) != NULL);
	ASSERT(&conn->request_info == mg_get_request_info(conn));
	ASSERT(!strcmp(conn->request_info.uri, "200"));
	ASSERT((p1 = read_conn(conn, &len1)) != NULL);
	ASSERT((p2 = read_file("unit_test.c", &len2)) != NULL);
	ASSERT(len1 == len2);
	ASSERT(memcmp(p1, p2, len1) == 0);
	mg_free(p1);
	mg_free(p2);
	mg_close_connection(conn);

	/* Fetch in-memory file, should succeed. */
	ASSERT((conn = mg_download("localhost",
	                           port,
	                           use_ssl,
	                           ebuf,
	                           sizeof(ebuf),
	                           "%s",
	                           "GET /blah HTTP/1.1\r\n\r\n")) != NULL);
	ASSERT((p1 = read_conn(conn, &len1)) != NULL);
	ASSERT(len1 == (int)strlen(inmemory_file_data));
	ASSERT(memcmp(p1, inmemory_file_data, len1) == 0);
	mg_free(p1);
	mg_close_connection(conn);

	/* Fetch in-memory data with no Content-Length, should succeed. */
	ASSERT((conn = mg_download("localhost",
	                           port,
	                           use_ssl,
	                           ebuf,
	                           sizeof(ebuf),
	                           "%s",
	                           "GET /data/all HTTP/1.1\r\n\r\n")) != NULL);
	ASSERT(conn->request_info.content_length == fetch_data_size);
	ASSERT((p1 = read_conn(conn, &len1)) != NULL);
	ASSERT(len1 == (int)fetch_data_size);
	ASSERT(memcmp(p1, fetch_data, len1) == 0);
	mg_free(p1);
	mg_close_connection(conn);

	/* Fetch in-memory data with no Content-Length, should succeed. */
	for (i = 0; i <= 1024 * /* 1024 * */ 8; i += (i < 2 ? 1 : i)) {
		ASSERT((conn = mg_download("localhost",
		                           port,
		                           use_ssl,
		                           ebuf,
		                           sizeof(ebuf),
		                           "GET /data/%i HTTP/1.1\r\n\r\n",
		                           i)) != NULL);
		ASSERT(conn->request_info.content_length == i);
		len1 = -1;
		p1 = read_conn(conn, &len1);
		if (i == 0) {
			ASSERT(len1 == 0);
			ASSERT(p1 == 0);
		} else if (i <= fetch_data_size) {
			ASSERT(p1 != NULL);
			ASSERT(len1 == i);
			ASSERT(memcmp(p1, fetch_data, len1) == 0);
		} else {
			ASSERT(p1 != NULL);
			ASSERT(len1 == i);
			ASSERT(memcmp(p1, fetch_data, fetch_data_size) == 0);
		}

		mg_free(p1);
		mg_close_connection(conn);
	}

	/* Fetch data with Content-Length, should succeed and return the defined
	 * length. */
	ASSERT((conn = mg_download(
	            "localhost",
	            port,
	            use_ssl,
	            ebuf,
	            sizeof(ebuf),
	            "POST /content_length HTTP/1.1\r\nContent-Length: %u\r\n\r\n%s",
	            (unsigned)strlen(test_data),
	            test_data)) != NULL);
	h = mg_get_header(conn, "Content-Length");
	ASSERT((h != NULL) && (atoi(h) == (int)strlen(test_data)));
	ASSERT((p1 = read_conn(conn, &len1)) != NULL);
	ASSERT(len1 == (int)strlen(test_data));
	ASSERT(conn->request_info.content_length == (int)strlen(test_data));
	ASSERT(memcmp(p1, test_data, len1) == 0);
	ASSERT(strcmp(conn->request_info.request_method, "HTTP/1.1") == 0);
	ASSERT(strcmp(conn->request_info.uri, "200") == 0);
	ASSERT(strcmp(conn->request_info.http_version, "OK") == 0);
	mg_free(p1);
	mg_close_connection(conn);

	/* A POST request without Content-Length set is only valid, if the request
	 * used Transfer-Encoding: chunked. Otherwise, it is an HTTP protocol
	 * violation. Here we send a chunked request, the reply is not chunked. */
	ASSERT((conn = mg_download("localhost",
	                           port,
	                           use_ssl,
	                           ebuf,
	                           sizeof(ebuf),
	                           "POST /content_length "
	                           "HTTP/1.1\r\n"
	                           "Transfer-Encoding: chunked\r\n"
	                           "\r\n%x\r\n%s\r\n0\r\n\r\n",
	                           (uint32_t)strlen(test_data),
	                           test_data)) != NULL);
	h = mg_get_header(conn, "Content-Length");
	ASSERT(h == NULL);
	ASSERT(conn->request_info.content_length == -1);
	ASSERT((p1 = read_conn(conn, &len1)) != NULL);
	ASSERT(len1 == (int)strlen(test_data));
	ASSERT(memcmp(p1, test_data, len1) == 0);
	mg_free(p1);
	mg_close_connection(conn);

	/* Another chunked POST request with different chunk sizes. */
	ASSERT((conn = mg_download("localhost",
	                           port,
	                           use_ssl,
	                           ebuf,
	                           sizeof(ebuf),
	                           "POST /content_length "
	                           "HTTP/1.1\r\n"
	                           "Transfer-Encoding: chunked\r\n\r\n"
	                           "2\r\n%c%c\r\n"
	                           "1\r\n%c\r\n"
	                           "2\r\n%c%c\r\n"
	                           "2\r\n%c%c\r\n"
	                           "%x\r\n%s\r\n"
	                           "0\r\n\r\n",
	                           test_data[0],
	                           test_data[1],
	                           test_data[2],
	                           test_data[3],
	                           test_data[4],
	                           test_data[5],
	                           test_data[6],
	                           (uint32_t)strlen(test_data + 7),
	                           test_data + 7)) != NULL);
	h = mg_get_header(conn, "Content-Length");
	ASSERT(h == NULL);
	ASSERT(conn->request_info.content_length == -1);
	ASSERT((p1 = read_conn(conn, &len1)) != NULL);
	ASSERT(len1 == (int)strlen(test_data));
	ASSERT(memcmp(p1, test_data, len1) == 0);
	mg_free(p1);
	mg_close_connection(conn);

	/* Test non existent */
	ASSERT((conn = mg_download("localhost",
	                           port,
	                           use_ssl,
	                           ebuf,
	                           sizeof(ebuf),
	                           "%s",
	                           "GET /non_exist HTTP/1.1\r\n\r\n")) != NULL);
	ASSERT(strcmp(conn->request_info.request_method, "HTTP/1.1") == 0);
	ASSERT(strcmp(conn->request_info.uri, "404") == 0);
	ASSERT(strcmp(conn->request_info.http_version, "Not Found") == 0);
	mg_close_connection(conn);

	if (use_ssl) {
#ifndef NO_SSL
		/* Test SSL redirect */
		ASSERT((conn = mg_download("localhost",
		                           atoi(HTTP_REDIRECT_PORT),
		                           0,
		                           ebuf,
		                           sizeof(ebuf),
		                           "%s",
		                           "GET /data/4711 HTTP/1.1\r\n\r\n")) != NULL);
		ASSERT(strcmp(conn->request_info.uri, "302") == 0);
		h = mg_get_header(conn, "Location");
		ASSERT(h != NULL);
		ASSERT(strcmp(h, "https://127.0.0.1:" HTTPS_PORT "/data/4711") == 0);
		mg_close_connection(conn);
#endif
	}

	/* Test new API */
	ebuf[0] = 1;
	conn = mg_connect_client("localhost", port, use_ssl, ebuf, sizeof(ebuf));
	ASSERT(conn != NULL);
	ASSERT(ebuf[0] == 0);
	ri = mg_get_request_info(conn);
	ASSERT(ri->content_length == 0);
	i = mg_get_response(conn, ebuf, sizeof(ebuf), 1000);
	ASSERT(ebuf[0] != 0);
	ri = mg_get_request_info(conn);
	ASSERT(ri->content_length == -1);
	mg_printf(conn, "GET /index.html HTTP/1.1\r\n");
	mg_printf(conn, "Host: www.example.com\r\n");
	mg_printf(conn, "\r\n");
	i = mg_get_response(conn, ebuf, sizeof(ebuf), 1000);
	ASSERT(ebuf[0] == 0);
	ri = mg_get_request_info(conn);
	ASSERT(ri->content_length > 0);
	mg_read(conn, ebuf, sizeof(ebuf));
	ASSERT(!strncmp(ebuf, "Error 404", 9));

	mg_close_connection(conn);

	/* Stop the test server */
	ut_mg_stop(ctx);
}

static int websocket_data_handler(const struct mg_connection *conn,
                                  int flags,
                                  char *data,
                                  size_t data_len,
                                  void *cbdata)
{
	(void)conn;
	(void)flags;
	(void)data;
	(void)data_len;
	(void)cbdata;
	return 1;
}

static void test_mg_websocket_client_connect(int use_ssl)
{
	struct mg_connection *conn;
	char ebuf[100];
	int port;
	struct mg_context *ctx;

	if (use_ssl)
		port = atoi(HTTPS_PORT);
	else
		port = atoi(HTTP_PORT);
	ASSERT((ctx = mg_start(&CALLBACKS, NULL, OPTIONS)) != NULL);

	/* Try to connect to our own server */
	/* Invalid port test */
	conn = mg_connect_websocket_client("localhost",
	                                   0,
	                                   use_ssl,
	                                   ebuf,
	                                   sizeof(ebuf),
	                                   "/",
	                                   "http://localhost",
	                                   (mg_websocket_data_handler)websocket_data_handler,
	                                   NULL,
	                                   NULL);
	ASSERT(conn == NULL);

	/* Should succeed, the default civetweb server should complete the handshake
	 */
	conn = mg_connect_websocket_client("localhost",
	                                   port,
	                                   use_ssl,
	                                   ebuf,
	                                   sizeof(ebuf),
	                                   "/",
	                                   "http://localhost",
	                                   (mg_websocket_data_handler)websocket_data_handler,
	                                   NULL,
	                                   NULL);
	ASSERT(conn != NULL);

	/* Try an external server test */
	port = 80;
	if (use_ssl) {
		port = 443;
	}

	/* Not a websocket server path */
	conn = mg_connect_websocket_client("websocket.org",
	                                   port,
	                                   use_ssl,
	                                   ebuf,
	                                   sizeof(ebuf),
	                                   "/",
	                                   "http://websocket.org",
	                                   (mg_websocket_data_handler)websocket_data_handler,
	                                   NULL,
	                                   NULL);
	ASSERT(conn == NULL);

	/* Invalid port test */
	conn = mg_connect_websocket_client("echo.websocket.org",
	                                   0,
	                                   use_ssl,
	                                   ebuf,
	                                   sizeof(ebuf),
	                                   "/",
	                                   "http://websocket.org",
	                                   (mg_websocket_data_handler)websocket_data_handler,
	                                   NULL,
	                                   NULL);
	ASSERT(conn == NULL);

	/* Should succeed, echo.websocket.org echos the data back */
	conn = mg_connect_websocket_client("echo.websocket.org",
	                                   port,
	                                   use_ssl,
	                                   ebuf,
	                                   sizeof(ebuf),
	                                   "/",
	                                   "http://websocket.org",
	                                   (mg_websocket_data_handler)websocket_data_handler,
	                                   NULL,
	                                   NULL);
	ASSERT(conn != NULL);

	ut_mg_stop(ctx);
}

static int alloc_printf(char **out_buf, char *buf, size_t size, char *fmt, ...)
{
	va_list ap;
	int ret = 0;
	va_start(ap, fmt);
	ret = alloc_vprintf(out_buf, buf, size, fmt, ap);
	va_end(ap);
	return ret;
}

#if defined(MG_LEGACY_INTERFACE)
static void test_mg_upload(void)
{
	static const char *boundary = "OOO___MY_BOUNDARY___OOO";
	struct mg_context *ctx;
#if 0
    struct mg_connection *conn;
    char ebuf[100], buf[20], *file2_data;
    int file2_len;
#endif
	char *file_data, *post_data;
	int file_len, post_data_len;

	ASSERT((ctx = mg_start(&CALLBACKS, NULL, OPTIONS)) != NULL);

	/* Upload one file */
	ASSERT((file_data = read_file("unit_test.c", &file_len)) != NULL);
	post_data = NULL;
	post_data_len = alloc_printf(&post_data,
                                 NULL,
	                             0,
	                             "--%s\r\n"
	                             "Content-Disposition: form-data; "
	                             "name=\"file\"; "
	                             "filename=\"%s\"\r\n\r\n"
	                             "%.*s\r\n"
	                             "--%s--\r\n",
	                             boundary,
	                             upload_filename,
	                             file_len,
	                             file_data,
	                             boundary);
	ASSERT(post_data_len > 0);

#if 0 /* TODO (bel): ... */
    ASSERT((conn = mg_download("localhost", atoi(HTTPS_PORT), 1,
        ebuf, sizeof(ebuf),
        "POST /upload?1 HTTP/1.1\r\n"
        "Content-Length: %d\r\n"
        "Content-Type: multipart/form-data; "
        "boundary=%s\r\n\r\n"
        "%.*s", post_data_len, boundary,
        post_data_len, post_data)) != NULL);
    mg_free(file_data), mg_free(post_data);
    ASSERT(mg_read(conn, buf, sizeof(buf)) == (int) strlen(upload_ok_message));
    ASSERT(memcmp(buf, upload_ok_message, strlen(upload_ok_message)) == 0);
    mg_close_connection(conn);

    /* Upload two files */
    ASSERT((file_data = read_file("include/civetweb.h", &file_len)) != NULL);
    ASSERT((file2_data = read_file("README.md", &file2_len)) != NULL);
    post_data = NULL;
    post_data_len = alloc_printf(&post_data, 0,
        /* First file */
        "--%s\r\n"
        "Content-Disposition: form-data; "
        "name=\"file\"; "
        "filename=\"%s\"\r\n\r\n"
        "%.*s\r\n"

        /* Second file */
        "--%s\r\n"
        "Content-Disposition: form-data; "
        "name=\"file\"; "
        "filename=\"%s\"\r\n\r\n"
        "%.*s\r\n"

        /* Final boundary */
        "--%s--\r\n",
        boundary, upload_filename,
        file_len, file_data,
        boundary, upload_filename2,
        file2_len, file2_data,
        boundary);
    ASSERT(post_data_len > 0);
    ASSERT((conn = mg_download("localhost", atoi(HTTPS_PORT), 1,
        ebuf, sizeof(ebuf),
        "POST /upload?2 HTTP/1.1\r\n"
        "Content-Length: %d\r\n"
        "Content-Type: multipart/form-data; "
        "boundary=%s\r\n\r\n"
        "%.*s", post_data_len, boundary,
        post_data_len, post_data)) != NULL);
    mg_free(file_data), mg_free(file2_data), mg_free(post_data);
    ASSERT(mg_read(conn, buf, sizeof(buf)) == (int) strlen(upload_ok_message));
    ASSERT(memcmp(buf, upload_ok_message, strlen(upload_ok_message)) == 0);
    mg_close_connection(conn);
#endif

	ut_mg_stop(ctx);
}
#endif

static void test_base64_encode(void)
{
	const char *in[] = {"a", "ab", "abc", "abcd", NULL};
	const char *out[] = {"YQ==", "YWI=", "YWJj", "YWJjZA=="};
	char buf[100];
	int i;

	for (i = 0; in [i] != NULL; i++) {
		base64_encode((unsigned char *)in[i], strlen(in[i]), buf);
		ASSERT(!strcmp(buf, out[i]));
	}
}

static void test_mg_get_var(void)
{
	static const char *post[] = {"a=1&&b=2&d&=&c=3%20&e=",
	                             "q=&st=2012%2F11%2F13+17%3A05&et=&team_id=",
	                             NULL};
	char buf[20];

	ASSERT(mg_get_var(post[0], strlen(post[0]), "a", buf, sizeof(buf)) == 1);
	ASSERT(buf[0] == '1' && buf[1] == '\0');
	ASSERT(mg_get_var(post[0], strlen(post[0]), "b", buf, sizeof(buf)) == 1);
	ASSERT(buf[0] == '2' && buf[1] == '\0');
	ASSERT(mg_get_var(post[0], strlen(post[0]), "c", buf, sizeof(buf)) == 2);
	ASSERT(buf[0] == '3' && buf[1] == ' ' && buf[2] == '\0');
	ASSERT(mg_get_var(post[0], strlen(post[0]), "e", buf, sizeof(buf)) == 0);
	ASSERT(buf[0] == '\0');

	ASSERT(mg_get_var(post[0], strlen(post[0]), "d", buf, sizeof(buf)) == -1);
	ASSERT(mg_get_var(post[0], strlen(post[0]), "c", buf, 2) == -2);

	ASSERT(mg_get_var(post[0], strlen(post[0]), "x", NULL, 10) == -2);
	ASSERT(mg_get_var(post[0], strlen(post[0]), "x", buf, 0) == -2);
	ASSERT(mg_get_var(post[1], strlen(post[1]), "st", buf, 16) == -2);
	ASSERT(mg_get_var(post[1], strlen(post[1]), "st", buf, 17) == 16);
}

static void test_set_throttle(void)
{
	ASSERT(set_throttle(NULL, 0x0a000001, "/") == 0);
	ASSERT(set_throttle("10.0.0.0/8=20", 0x0a000001, "/") == 20);
	ASSERT(set_throttle("10.0.0.0/8=0.5k", 0x0a000001, "/") == 512);
	ASSERT(set_throttle("10.0.0.0/8=17m", 0x0a000001, "/") == 1048576 * 17);
	ASSERT(set_throttle("10.0.0.0/8=1x", 0x0a000001, "/") == 0);
	ASSERT(set_throttle("10.0.0.0/8=5,0.0.0.0/0=10", 0x0a000001, "/") == 10);
	ASSERT(set_throttle("10.0.0.0/8=5,/foo/**=7", 0x0a000001, "/index") == 5);
	ASSERT(set_throttle("10.0.0.0/8=5,/foo/**=7", 0x0a000001, "/foo/x") == 7);
	ASSERT(set_throttle("10.0.0.0/8=5,/foo/**=7", 0x0b000001, "/foxo/x") == 0);
	ASSERT(set_throttle("10.0.0.0/8=5,*=1", 0x0b000001, "/foxo/x") == 1);
}

static void test_next_option(void)
{
	const char *p, *list = "x/8,/y**=1;2k,z";
	struct vec a, b;
	int i;

	ASSERT(next_option(NULL, &a, &b) == NULL);
	for (i = 0, p = list; (p = next_option(p, &a, &b)) != NULL; i++) {
		ASSERT(i != 0 || (a.ptr == list && a.len == 3 && b.len == 0));
		ASSERT(i != 1 || (a.ptr == list + 4 && a.len == 4 &&
		                  b.ptr == list + 9 && b.len == 4));
		ASSERT(i != 2 || (a.ptr == list + 14 && a.len == 1 && b.len == 0));
	}
}

#if defined(USE_LUA)
static void check_lua_expr(lua_State *L, const char *expr, const char *value)
{
	const char *v, *var_name = "myVar";
	char buf[100];

	mg_snprintf(buf, sizeof(buf), "%s = %s", var_name, expr);
	(void)luaL_dostring(L, buf);
	lua_getglobal(L, var_name);
	v = lua_tostring(L, -1);
	ASSERT((value == NULL && v == NULL) ||
	       (value != NULL && v != NULL && !strcmp(value, v)));
}

static void test_lua(void)
{
	static struct mg_connection conn;
	static struct mg_context ctx;

	char http_request[] = "POST /foo/bar HTTP/1.1\r\n"
	                      "Content-Length: 12\r\n"
	                      "Connection: close\r\n\r\nhello world!";
	lua_State *L = luaL_newstate();

	conn.ctx = &ctx;
	conn.buf = http_request;
	conn.buf_size = conn.data_len = strlen(http_request);
	conn.request_len =
	    parse_http_message(conn.buf, conn.data_len, &conn.request_info);
	conn.content_len = conn.data_len - conn.request_len;

	prepare_lua_environment(&ctx, &conn, NULL, L, "unit_test", LUA_ENV_TYPE_PLAIN_LUA_PAGE);
	ASSERT(lua_gettop(L) == 4);

	check_lua_expr(L, "'hi'", "hi");
	check_lua_expr(L, "mg.request_info.request_method", "POST");
	check_lua_expr(L, "mg.request_info.uri", "/foo/bar");
	check_lua_expr(L, "mg.request_info.num_headers", "2");
	check_lua_expr(L, "mg.request_info.remote_ip", "0");
	check_lua_expr(L, "mg.request_info.http_headers['Content-Length']", "12");
	check_lua_expr(L, "mg.request_info.http_headers['Connection']", "close");
	(void)luaL_dostring(L, "post = mg.read()");
	check_lua_expr(L, "# post", "12");
	check_lua_expr(L, "post", "hello world!");
	lua_close(L);
}
#endif

static void test_mg_stat(void)
{
	static struct mg_context ctx;
	struct file file = STRUCT_FILE_INITIALIZER;
	ASSERT(!mg_stat(fc(&ctx), " does not exist ", &file));
}

static void test_skip_quoted(void)
{
	char x[] = "a=1, b=2, c='hi \' there', d='here\\, there'", *s = x, *p;

	p = skip_quoted(&s, ", ", ", ", 0);
	ASSERT(p != NULL && !strcmp(p, "a=1"));

	p = skip_quoted(&s, ", ", ", ", 0);
	ASSERT(p != NULL && !strcmp(p, "b=2"));

	p = skip_quoted(&s, ",", " ", 0);
	ASSERT(p != NULL && !strcmp(p, "c='hi \' there'"));

	p = skip_quoted(&s, ",", " ", '\\');
	ASSERT(p != NULL && !strcmp(p, "d='here, there'"));
	ASSERT(*s == 0);
}

static void test_alloc_vprintf(void)
{
	char buf[MG_BUF_LEN], *p = buf;

	ASSERT(alloc_printf(&p, buf, sizeof(buf), "%s", "hi") == 2);
	ASSERT(p == buf);
	ASSERT(alloc_printf(&p, buf, sizeof(buf), "%s", "") == 0);
	ASSERT(alloc_printf(&p, buf, sizeof(buf), "") == 0);

	/* Pass small buffer, make sure alloc_printf allocates */
	ASSERT(alloc_printf(&p, buf, 1, "%s", "hello") == 5);
	ASSERT(p != buf);
	mg_free(p);
}

static void test_request_replies(void)
{
	char ebuf[100];
	int i;
	struct mg_connection *conn;
	struct mg_context *ctx;
	static struct {
		const char *request, *reply_regex;
	} tests[] = {
	    {"GET hello.txt HTTP/1.0\r\nRange: bytes=3-5\r\n\r\n",
	     "^HTTP/1.1 206 Partial Content"},
	    {NULL, NULL},
	};

	ASSERT((ctx = mg_start(&CALLBACKS, NULL, OPTIONS)) != NULL);
	for (i = 0; tests[i].request != NULL; i++) {
		ASSERT((conn = mg_download("localhost",
		                           atoi(HTTP_PORT),
		                           0,
		                           ebuf,
		                           sizeof(ebuf),
		                           "%s",
		                           tests[i].request)) != NULL);
		mg_close_connection(conn);
	}
	ut_mg_stop(ctx);

#ifndef NO_SSL
	ASSERT((ctx = mg_start(&CALLBACKS, NULL, OPTIONS)) != NULL);
	for (i = 0; tests[i].request != NULL; i++) {
		ASSERT((conn = mg_download("localhost",
		                           atoi(HTTPS_PORT),
		                           1,
		                           ebuf,
		                           sizeof(ebuf),
		                           "%s",
		                           tests[i].request)) != NULL);
		mg_close_connection(conn);
	}
	ut_mg_stop(ctx);
#endif
}

static int request_test_handler(struct mg_connection *conn, void *cbdata)
{
	int i;
	char chunk_data[32];

	ASSERT(cbdata == (void *)7);
	strcpy(chunk_data, "123456789A123456789B123456789C");

	mg_printf(conn,
	          "HTTP/1.1 200 OK\r\n"
	          "Transfer-Encoding: chunked\r\n"
	          "Content-Type: text/plain\r\n\r\n");

	for (i = 0; i < 20; i++) {
		mg_printf(conn, "%x\r\n", i);
		mg_write(conn, chunk_data, i);
		mg_printf(conn, "\r\n");
	}

	mg_printf(conn, "0\r\n\r\n");

	return 1;
}


static void test_request_handlers(void)
{
	char ebuf[100];
	struct mg_context *ctx;
	struct mg_connection *conn;
	char uri[64];
	int i;
	const char *request = "GET /U7 HTTP/1.0\r\n\r\n";

	ctx = mg_start(NULL, NULL, OPTIONS);
	ASSERT(ctx != NULL);

	for (i = 0; i < 1000; i++) {
		sprintf(uri, "/U%u", i);
		mg_set_request_handler(ctx, uri, request_test_handler, NULL);
	}
	for (i = 500; i < 800; i++) {
		sprintf(uri, "/U%u", i);
		mg_set_request_handler(ctx, uri, NULL, (void *)1);
	}
	for (i = 600; i >= 0; i--) {
		sprintf(uri, "/U%u", i);
		mg_set_request_handler(ctx, uri, NULL, (void *)2);
	}
	for (i = 750; i <= 1000; i++) {
		sprintf(uri, "/U%u", i);
		mg_set_request_handler(ctx, uri, NULL, (void *)3);
	}
	for (i = 5; i < 9; i++) {
		sprintf(uri, "/U%u", i);
		mg_set_request_handler(ctx, uri, request_test_handler, (void *)(intptr_t)i);
	}

	conn = mg_download(
	    "localhost", atoi(HTTP_PORT), 0, ebuf, sizeof(ebuf), "%s", request);
	ASSERT(conn != NULL);
	mg_sleep(1000);
	mg_close_connection(conn);

	ut_mg_stop(ctx);
}

static int api_callback(struct mg_connection *conn)
{
	const struct mg_request_info *ri = mg_get_request_info(conn);
	char post_data[100] = "";

	ASSERT(ri->user_data == (void *)123);
	ASSERT(ri->num_headers == 2);
	ASSERT(strcmp(mg_get_header(conn, "host"), "blah.com") == 0);
	ASSERT(mg_read(conn, post_data, sizeof(post_data)) == 3);
	ASSERT(memcmp(post_data, "b=1", 3) == 0);
	ASSERT(ri->query_string != NULL);
	ASSERT(ri->remote_addr[0] != 0);
	ASSERT(ri->remote_port > 0);
	ASSERT(strcmp(ri->http_version, "1.0") == 0);

	mg_printf(conn, "HTTP/1.0 200 OK\r\n\r\n");
	return 1;
}

static void test_api_calls(void)
{
	char ebuf[100];
	struct mg_callbacks callbacks;
	struct mg_connection *conn;
	struct mg_context *ctx;
	static const char *request =
	    "POST /?a=%20&b=&c=xx HTTP/1.0\r\n"
	    "Host:  blah.com\n"     /* More spaces before */
	    "content-length: 3\r\n" /* Lower case header name */
	    "\r\nb=123456"; /* Content size > content-length, test for mg_read() */

	memset(&callbacks, 0, sizeof(callbacks));
	callbacks.begin_request = api_callback;
	ASSERT((ctx = mg_start(&callbacks, (void *)123, OPTIONS)) != NULL);
	ASSERT((conn = mg_download("localhost",
	                           atoi(HTTP_PORT),
	                           0,
	                           ebuf,
	                           sizeof(ebuf),
	                           "%s",
	                           request)) != NULL);
	mg_close_connection(conn);
	ut_mg_stop(ctx);
}

static void test_url_decode(void)
{
	char buf[100];

	ASSERT(mg_url_decode("foo", 3, buf, 3, 0) == -1); /* No space for \0 */
	ASSERT(mg_url_decode("foo", 3, buf, 4, 0) == 3);
	ASSERT(strcmp(buf, "foo") == 0);

	ASSERT(mg_url_decode("a+", 2, buf, sizeof(buf), 0) == 2);
	ASSERT(strcmp(buf, "a+") == 0);

	ASSERT(mg_url_decode("a+", 2, buf, sizeof(buf), 1) == 2);
	ASSERT(strcmp(buf, "a ") == 0);

	ASSERT(mg_url_decode("%61", 1, buf, sizeof(buf), 1) == 1);
	ASSERT(strcmp(buf, "%") == 0);

	ASSERT(mg_url_decode("%61", 2, buf, sizeof(buf), 1) == 2);
	ASSERT(strcmp(buf, "%6") == 0);

	ASSERT(mg_url_decode("%61", 3, buf, sizeof(buf), 1) == 1);
	ASSERT(strcmp(buf, "a") == 0);
}

static void test_mg_strcasestr(void)
{
	static const char *big1 = "abcdef";
	ASSERT(mg_strcasestr("Y", "X") == NULL);
	ASSERT(mg_strcasestr("Y", "y") != NULL);
	ASSERT(mg_strcasestr(big1, "X") == NULL);
	ASSERT(mg_strcasestr(big1, "CD") == big1 + 2);
	ASSERT(mg_strcasestr("aa", "AAB") == NULL);
}

static void test_mg_get_cookie(void)
{
	char buf[20];

	ASSERT(mg_get_cookie("", "foo", NULL, sizeof(buf)) == -2);
	ASSERT(mg_get_cookie("", "foo", buf, 0) == -2);
	ASSERT(mg_get_cookie("", "foo", buf, sizeof(buf)) == -1);
	ASSERT(mg_get_cookie("", NULL, buf, sizeof(buf)) == -1);
	ASSERT(mg_get_cookie("a=1; b=2; c; d", "a", buf, sizeof(buf)) == 1);
	ASSERT(strcmp(buf, "1") == 0);
	ASSERT(mg_get_cookie("a=1; b=2; c; d", "b", buf, sizeof(buf)) == 1);
	ASSERT(strcmp(buf, "2") == 0);
	ASSERT(mg_get_cookie("a=1; b=123", "b", buf, sizeof(buf)) == 3);
	ASSERT(strcmp(buf, "123") == 0);
	ASSERT(mg_get_cookie("a=1; b=2; c; d", "c", buf, sizeof(buf)) == -1);
}

static void test_strtoll(void)
{
	ASSERT(strtoll("0", NULL, 10) == 0);
	ASSERT(strtoll("123", NULL, 10) == 123);
	ASSERT(strtoll("-34", NULL, 10) == -34);
	ASSERT(strtoll("3566626116", NULL, 10) == 3566626116);
}

static void test_parse_port_string(void)
{
	static const char *valid[] = {
		"0",
		"1",
		"1s",
		"1r",
		"1.2.3.4:1",
		"1.2.3.4:1s",
		"1.2.3.4:1r",
#if defined(USE_IPV6)
		"[::1]:123",
		"[3ffe:2a00:100:7031::1]:900",
#endif
		NULL
	};
	static const char *invalid[] = {
	    "99999", "1k", "1.2.3", "1.2.3.4:", "1.2.3.4:2p", NULL};
	struct socket so;
	struct vec vec;
	int ipv;
	int i;

	for (i = 0; valid[i] != NULL; i++) {
		vec.ptr = valid[i];
		vec.len = strlen(vec.ptr);
		ASSERT(parse_port_string(&vec, &so, &ipv) != 0);
	}

	for (i = 0; invalid[i] != NULL; i++) {
		vec.ptr = invalid[i];
		vec.len = strlen(vec.ptr);
		ASSERT(parse_port_string(&vec, &so, &ipv) == 0);
	}
}

static void test_md5(void)
{

	md5_state_t md5_state;
	unsigned char md5_val[16 + 1];
	char md5_str[32 + 1];
	const char *test_str = "The quick brown fox jumps over the lazy dog";

	md5_val[16] = 0;
	md5_init(&md5_state);
	md5_finish(&md5_state, md5_val);
	ASSERT(strcmp((const char *)md5_val,
	              "\xd4\x1d\x8c\xd9\x8f\x00\xb2\x04\xe9"
	              "\x80\x09\x98\xec\xf8\x42\x7e") == 0);
	sprintf(md5_str,
	        "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
	        md5_val[0],
	        md5_val[1],
	        md5_val[2],
	        md5_val[3],
	        md5_val[4],
	        md5_val[5],
	        md5_val[6],
	        md5_val[7],
	        md5_val[8],
	        md5_val[9],
	        md5_val[10],
	        md5_val[11],
	        md5_val[12],
	        md5_val[13],
	        md5_val[14],
	        md5_val[15]);
	ASSERT(strcmp(md5_str, "d41d8cd98f00b204e9800998ecf8427e") == 0);

	mg_md5(md5_str, "", NULL);
	ASSERT(strcmp(md5_str, "d41d8cd98f00b204e9800998ecf8427e") == 0);

	md5_init(&md5_state);
	md5_append(&md5_state, (const md5_byte_t *)test_str, strlen(test_str));
	md5_finish(&md5_state, md5_val);
	sprintf(md5_str,
	        "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
	        md5_val[0],
	        md5_val[1],
	        md5_val[2],
	        md5_val[3],
	        md5_val[4],
	        md5_val[5],
	        md5_val[6],
	        md5_val[7],
	        md5_val[8],
	        md5_val[9],
	        md5_val[10],
	        md5_val[11],
	        md5_val[12],
	        md5_val[13],
	        md5_val[14],
	        md5_val[15]);
	ASSERT(strcmp(md5_str, "9e107d9d372bb6826bd81d3542a419d6") == 0);

	mg_md5(md5_str, test_str, NULL);
	ASSERT(strcmp(md5_str, "9e107d9d372bb6826bd81d3542a419d6") == 0);

	mg_md5(md5_str,
	       "The",
	       " ",
	       "quick brown fox",
	       "",
	       " jumps ",
	       "over the lazy dog",
	       "",
	       "",
	       NULL);
	ASSERT(strcmp(md5_str, "9e107d9d372bb6826bd81d3542a419d6") == 0);

	mg_md5(md5_str, "civetweb", NULL);
	ASSERT(strcmp(md5_str, "95c098bd85b619b24a83d9cea5e8ba54") == 0);
}

int __cdecl main(void)
{

	char buffer[512];
	FILE *f;
	struct mg_context *ctx;
	int i;

	/* print headline */
	printf("Civetweb %s unit test\n", mg_version());
#if defined(_WIN32)
	GetCurrentDirectoryA(sizeof(buffer), buffer);
#else
	getcwd(buffer, sizeof(buffer));
#endif
	printf("Test directory is \"%s\"\n",
	       buffer); /* should be the "test" directory */
	f = fopen("hello.txt", "r");
	if (f) {
		fclose(f);
	} else {
		printf("Error: Test directory does not contain hello.txt\n");
	}
	f = fopen("unit_test.c", "r");
	if (f) {
		fclose(f);
	} else {
		printf("Error: Test directory does not contain unit_test.c\n");
	}

	/* test local functions */
	test_parse_port_string();
	test_mg_strcasestr();
	test_alloc_vprintf();
	test_base64_encode();
	test_match_prefix();
	test_remove_double_dots();
	test_should_keep_alive();
	test_parse_http_message();
	test_mg_get_var();
	test_set_throttle();
	test_next_option();
	test_mg_stat();
	test_skip_quoted();
	test_url_decode();
	test_mg_get_cookie();
	test_strtoll();
	test_md5();

	/* start stop server */
	ctx = mg_start(NULL, NULL, OPTIONS);
	REQUIRE(ctx != NULL);
	mg_sleep(1000);
	ut_mg_stop(ctx);

	/* create test data */
	fetch_data = (char *)mg_malloc(fetch_data_size);
	for (i = 0; i < fetch_data_size; i++) {
		fetch_data[i] = 'a' + i % 10;
	}

	/* tests with network access */
	init_CALLBACKS();
	test_mg_download(0);
#ifndef NO_SSL
	test_mg_download(1);
#endif

	test_mg_websocket_client_connect(0);
#ifndef NO_SSL
	test_mg_websocket_client_connect(1);
#endif

#if defined(MG_LEGACY_INTERFACE)
	test_mg_upload();
#endif
	test_request_replies();
	test_api_calls();
	test_request_handlers();

#if defined(USE_LUA)
	test_lua();
#endif

	/* test completed */
	mg_free(fetch_data);

#ifdef MEMORY_DEBUGGING
	{
		extern unsigned long mg_memory_debug_blockCount;
		extern unsigned long mg_memory_debug_totalMemUsed;

		printf("MEMORY DEBUGGING: %lu %lu\n",
		       mg_memory_debug_blockCount,
		       mg_memory_debug_totalMemUsed);
	}
#endif

	printf("TOTAL TESTS: %d, FAILED: %d\n", s_total_tests, s_failed_tests);
	return s_failed_tests == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
