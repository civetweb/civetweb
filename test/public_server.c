/* Copyright (c) 2015 the Civetweb developers
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

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "public_server.h"
#include <civetweb.h>

#if defined(_WIN32)
#include <Windows.h>
#define mg_Sleep(x) (Sleep(x * 1000))
#else
#include <unistd.h>
#define mg_Sleep(x) (sleep(x))
#endif

/* This unit test file uses the excellent Check unit testing library.
 * The API documentation is available here:
 * http://check.sourceforge.net/doc/check_html/index.html
 */


START_TEST(test_the_test_environment)
{
	char wd[300];
	char buf[500];
	FILE *f;
	struct stat st;
	int ret;

	memset(wd, 0, sizeof(wd));
	memset(buf, 0, sizeof(buf));

/* Get the current working directory */
#ifdef _WIN32
	(void)GetCurrentDirectoryA(sizeof(wd), wd);
	wd[sizeof(wd) - 1] = 0;
#else
	(void)getcwd(wd, sizeof(wd));
	wd[sizeof(wd) - 1] = 0;
#endif

/* Check the pem file */
#ifdef _WIN32
	strcpy(buf, wd);
	strcat(buf, "..\\..\\..\\resources\\ssl_cert.pem");
	f = fopen(buf, "rb");
#else
	strcpy(buf, wd);
	strcat(buf, "../../resources/ssl_cert.pem");
	f = fopen(buf, "r");
#endif

	if (f) {
		fclose(f);
	} else {
		fprintf(stderr, "%s not found", buf);
	}

/* Check the test dir */
#ifdef _WIN32
	strcpy(buf, wd);
	strcat(buf, "\\test");
#else
	strcpy(buf, wd);
	strcat(buf, "/test");
#endif

	memset(&st, 0, sizeof(st));
	ret = stat(buf, &st);

	if (ret) {
		fprintf(stderr, "%s not found", buf);
	}
}
END_TEST


static int log_msg_func(const struct mg_connection *conn, const char *message)
{
	struct mg_context *ctx;
	char *ud;

	ck_assert(conn != NULL);
	ctx = mg_get_context(conn);
	ck_assert(ctx != NULL);
	ud = (char *)mg_get_user_data(ctx);

	strncpy(ud, message, 255);
	ud[255] = 0;
	return 1;
}


START_TEST(test_mg_start_stop_http_server)
{
	struct mg_context *ctx;
	const char *OPTIONS[] = {
	    "document_root", ".", "listening_ports", "8080", NULL,
	};
	size_t ports_cnt;
	int ports[16];
	int ssl[16];
	struct mg_callbacks callbacks;
	char errmsg[256];

	memset(ports, 0, sizeof(ports));
	memset(ssl, 0, sizeof(ssl));
	memset(&callbacks, 0, sizeof(callbacks));
	memset(errmsg, 0, sizeof(errmsg));

	callbacks.log_message = log_msg_func;

	ctx = mg_start(&callbacks, (void *)errmsg, OPTIONS);
	mg_Sleep(1);
	ck_assert_str_eq(errmsg, "");
	ck_assert(ctx != NULL);

	ports_cnt = mg_get_ports(ctx, 16, ports, ssl);
	ck_assert_uint_eq(ports_cnt, 1);
	ck_assert_int_eq(ports[0], 8080);
	ck_assert_int_eq(ssl[0], 0);
	ck_assert_int_eq(ports[1], 0);
	ck_assert_int_eq(ssl[1], 0);

	mg_Sleep(1);
	mg_stop(ctx);
}
END_TEST


START_TEST(test_mg_start_stop_https_server)
{
	struct mg_context *ctx;
	const char *OPTIONS[] = {
	    "document_root",
	    ".",
	    "listening_ports",
	    "8080,8443s",
	    "ssl_certificate",
#ifdef _WIN32
	    "..\\..\\..\\resources/ssl_cert.pem", // TODO: the different paths used
	                                          // in the different test system is
	                                          // an unsolved problem
#else
	    "../../resources/ssl_cert.pem", // TODO: fix path in CI test environment
#endif
	    NULL,
	};
	size_t ports_cnt;
	int ports[16];
	int ssl[16];
	struct mg_callbacks callbacks;
	char errmsg[256];

	memset(ports, 0, sizeof(ports));
	memset(ssl, 0, sizeof(ssl));
	memset(&callbacks, 0, sizeof(callbacks));
	memset(errmsg, 0, sizeof(errmsg));

	callbacks.log_message = log_msg_func;

	ctx = mg_start(&callbacks, (void *)errmsg, OPTIONS);
	mg_Sleep(1);
	ck_assert_str_eq(errmsg, "");
	ck_assert(ctx != NULL);

	ports_cnt = mg_get_ports(ctx, 16, ports, ssl);
	ck_assert_uint_eq(ports_cnt, 2);
	ck_assert_int_eq(ports[0], 8080);
	ck_assert_int_eq(ssl[0], 0);
	ck_assert_int_eq(ports[1], 8443);
	ck_assert_int_eq(ssl[1], 1);
	ck_assert_int_eq(ports[2], 0);
	ck_assert_int_eq(ssl[2], 0);

	mg_Sleep(1);
	mg_stop(ctx);
}
END_TEST


static struct mg_context *g_ctx;

static int request_test_handler(struct mg_connection *conn, void *cbdata)
{
	int i;
	char chunk_data[32];
	const struct mg_request_info *ri;
	struct mg_context *ctx;
	void *ud, *cud;

	ctx = mg_get_context(conn);
	ud = mg_get_user_data(ctx);
	ri = mg_get_request_info(conn);

	ck_assert(ri != NULL);
	ck_assert(ctx == g_ctx);
	ck_assert(ud == &g_ctx);

	mg_set_user_connection_data(conn, (void *)6543);
	cud = mg_get_user_connection_data(conn);
	ck_assert(cud == (void *)6543);

	ck_assert(cbdata == (void *)7);
	strcpy(chunk_data, "123456789A123456789B123456789C");

	mg_printf(conn,
	          "HTTP/1.1 200 OK\r\n"
	          "Transfer-Encoding: chunked\r\n"
	          "Content-Type: text/plain\r\n\r\n");

	for (i = 1; i <= 10; i++) {
		mg_printf(conn, "%x\r\n", i);
		mg_write(conn, chunk_data, (unsigned)i);
		mg_printf(conn, "\r\n");
	}

	mg_printf(conn, "0\r\n\r\n");

	return 1;
}


START_TEST(test_request_handlers)
{
	char ebuf[100];
	struct mg_context *ctx;
	struct mg_connection *conn;
	const struct mg_request_info *ri;
	char uri[64];
	char buf[1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10 + 8];
	const char *expected =
	    "112123123412345123456123456712345678123456789123456789A";
	int i;
	const char *request = "GET /U7 HTTP/1.0\r\n\r\n";
#if defined(USE_IPV6) && defined(NO_SSL)
	const char *HTTP_PORT = "8084,[::]:8086";
	short ipv4_port = 8084;
	short ipv6_port = 8086;
#elif !defined(USE_IPV6) && defined(NO_SSL)
	const char *HTTP_PORT = "8084";
	short ipv4_port = 8084;
#elif defined(USE_IPV6) && !defined(NO_SSL)
	const char *HTTP_PORT = "8084,[::]:8086,8194r,[::]:8196r,8094s,[::]:8096s";
	short ipv4_port = 8084;
	short ipv4s_port = 8094;
	short ipv4r_port = 8194;
	short ipv6_port = 8086;
	short ipv6s_port = 8096;
	short ipv6r_port = 8196;
#elif !defined(USE_IPV6) && !defined(NO_SSL)
	const char *HTTP_PORT = "8084,8194r,8094s";
	short ipv4_port = 8084;
	short ipv4s_port = 8094;
	short ipv4r_port = 8194;
#endif

	const char *OPTIONS[8]; /* initializer list here is rejected by CI test */
	const char *opt;
	FILE *f;

	memset((void *)OPTIONS, 0, sizeof(OPTIONS));
	OPTIONS[0] = "listening_ports";
	OPTIONS[1] = HTTP_PORT;
	OPTIONS[2] = "document_root";
	OPTIONS[3] = ".";
#ifndef NO_SSL
	OPTIONS[4] = "ssl_certificate";
#ifdef _WIN32
	OPTIONS[5] = "..\\..\\..\\resources/ssl_cert.pem"; // TODO: the different
	                                                   // paths used in the
	                                                   // different test system
	                                                   // is an unsolved problem
#else
	OPTIONS[5] =
	    "../../resources/ssl_cert.pem"; // TODO: fix path in CI test environment
#endif
#endif
	ck_assert(OPTIONS[sizeof(OPTIONS) / sizeof(OPTIONS[0]) - 1] == NULL);
	ck_assert(OPTIONS[sizeof(OPTIONS) / sizeof(OPTIONS[0]) - 2] == NULL);

	ctx = mg_start(NULL, &g_ctx, OPTIONS);
	ck_assert(ctx != NULL);
	g_ctx = ctx;

	opt = mg_get_option(ctx, "listening_ports");
	ck_assert_str_eq(opt, HTTP_PORT);
	opt = mg_get_option(ctx, "cgi_environment");
	ck_assert_str_eq(opt, "");
	opt = mg_get_option(ctx, "unknown_option_name");
	ck_assert(opt == NULL);

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
		mg_set_request_handler(
		    ctx, uri, request_test_handler, (void *)(ptrdiff_t)i);
	}


	/* Try to load non existing file */
	conn = mg_download("localhost",
	                   ipv4_port,
	                   0,
	                   ebuf,
	                   sizeof(ebuf),
	                   "%s",
	                   "GET /file/not/found HTTP/1.0\r\n\r\n");
	ck_assert(conn != NULL);
	ri = mg_get_request_info(conn);

	ck_assert(ri != NULL);
	ck_assert_str_eq(ri->uri, "404");
	mg_close_connection(conn);


	/* Get data from callback */
	conn = mg_download(
	    "localhost", ipv4_port, 0, ebuf, sizeof(ebuf), "%s", request);
	ck_assert(conn != NULL);
	ri = mg_get_request_info(conn);

	ck_assert(ri != NULL);
	ck_assert_str_eq(ri->uri, "200");
	i = mg_read(conn, buf, sizeof(buf));
	ck_assert_int_eq(i, (int)strlen(expected));
	buf[i] = 0;
	ck_assert_str_eq(buf, expected);
	mg_close_connection(conn);


	/* Get data from callback using http://127.0.0.1 */
	conn = mg_download(
	    "127.0.0.1", ipv4_port, 0, ebuf, sizeof(ebuf), "%s", request);
	ck_assert(conn != NULL);
	ri = mg_get_request_info(conn);

	ck_assert(ri != NULL);
	ck_assert_str_eq(ri->uri, "200");
	i = mg_read(conn, buf, sizeof(buf));
	ck_assert_int_eq(i, (int)strlen(expected));
	buf[i] = 0;
	ck_assert_str_eq(buf, expected);
	mg_close_connection(conn);


#if defined(USE_IPV6)
	/* Get data from callback using http://[::1] */
	conn =
	    mg_download("[::1]", ipv6_port, 0, ebuf, sizeof(ebuf), "%s", request);
	ck_assert(conn != NULL);
	ri = mg_get_request_info(conn);

	ck_assert(ri != NULL);
	ck_assert_str_eq(ri->uri, "200");
	i = mg_read(conn, buf, sizeof(buf));
	ck_assert_int_eq(i, (int)strlen(expected));
	buf[i] = 0;
	ck_assert_str_eq(buf, expected);
	mg_close_connection(conn);
#endif


#if !defined(NO_SSL)
	/* Get data from callback using https://127.0.0.1 */
	conn = mg_download(
	    "127.0.0.1", ipv4s_port, 1, ebuf, sizeof(ebuf), "%s", request);
	ck_assert(conn != NULL);
	ri = mg_get_request_info(conn);

	ck_assert(ri != NULL);
	ck_assert_str_eq(ri->uri, "200");
	i = mg_read(conn, buf, sizeof(buf));
	ck_assert_int_eq(i, (int)strlen(expected));
	buf[i] = 0;
	ck_assert_str_eq(buf, expected);
	mg_close_connection(conn);

	/* Get redirect from callback using http://127.0.0.1 */
	conn = mg_download(
	    "127.0.0.1", ipv4r_port, 0, ebuf, sizeof(ebuf), "%s", request);
	ck_assert(conn != NULL);
	ri = mg_get_request_info(conn);

	ck_assert(ri != NULL);
	ck_assert_str_eq(ri->uri, "302");
	i = mg_read(conn, buf, sizeof(buf));
	ck_assert_int_eq(i, -1);
	mg_close_connection(conn);
#endif


#if defined(USE_IPV6) && !defined(NO_SSL)
	/* Get data from callback using https://[::1] */
	conn =
	    mg_download("[::1]", ipv6s_port, 1, ebuf, sizeof(ebuf), "%s", request);
	ck_assert(conn != NULL);
	ri = mg_get_request_info(conn);

	ck_assert(ri != NULL);
	ck_assert_str_eq(ri->uri, "200");
	i = mg_read(conn, buf, sizeof(buf));
	ck_assert_int_eq(i, (int)strlen(expected));
	buf[i] = 0;
	ck_assert_str_eq(buf, expected);
	mg_close_connection(conn);

	/* Get redirect from callback using http://127.0.0.1 */
	conn =
	    mg_download("[::1]", ipv6r_port, 0, ebuf, sizeof(ebuf), "%s", request);
	ck_assert(conn != NULL);
	ri = mg_get_request_info(conn);

	ck_assert(ri != NULL);
	ck_assert_str_eq(ri->uri, "302");
	i = mg_read(conn, buf, sizeof(buf));
	ck_assert_int_eq(i, -1);
	mg_close_connection(conn);
#endif


/* It seems to be impossible to find out what the actual working
 * directory of the CI test environment is. Before breaking another
 * dozen of builds by trying blindly with different paths, just
 * create the file here */
#ifdef _WIN32
	f = fopen("test.txt", "wb");
#else
	f = fopen("test.txt", "w");
#endif
	fwrite("simple text file\n", 17, 1, f);
	fclose(f);


	/* Get static data */
	conn = mg_download("localhost",
	                   ipv4_port,
	                   0,
	                   ebuf,
	                   sizeof(ebuf),
	                   "%s",
	                   "GET /test.txt HTTP/1.0\r\n\r\n");
	ck_assert(conn != NULL);
	ri = mg_get_request_info(conn);

	ck_assert(ri != NULL);
	ck_assert_str_eq(ri->uri, "200");
	i = mg_read(conn, buf, sizeof(buf));
	ck_assert_int_eq(i, 17);
	if ((i >= 0) && (i < (int)sizeof(buf))) {
		buf[i] = 0;
	}
	ck_assert_str_eq(buf, "simple text file\n");
	mg_close_connection(conn);


	/* Get directory listing */
	conn = mg_download("localhost",
	                   ipv4_port,
	                   0,
	                   ebuf,
	                   sizeof(ebuf),
	                   "%s",
	                   "GET / HTTP/1.0\r\n\r\n");
	ck_assert(conn != NULL);
	ri = mg_get_request_info(conn);

	ck_assert(ri != NULL);
	ck_assert_str_eq(ri->uri, "200");
	i = mg_read(conn, buf, sizeof(buf));
	ck_assert(i > 6);
	buf[6] = 0;
	ck_assert_str_eq(buf, "<html>");
	mg_close_connection(conn);


	/* POST to static file (will not work) */
	conn = mg_download("localhost",
	                   ipv4_port,
	                   0,
	                   ebuf,
	                   sizeof(ebuf),
	                   "%s",
	                   "POST /test.txt HTTP/1.0\r\n\r\n");
	ck_assert(conn != NULL);
	ri = mg_get_request_info(conn);

	ck_assert(ri != NULL);
	ck_assert_str_eq(ri->uri, "405");
	i = mg_read(conn, buf, sizeof(buf));
	ck_assert(i >= 29);
	buf[29] = 0;
	ck_assert_str_eq(buf, "Error 405: Method Not Allowed");
	mg_close_connection(conn);


	/* PUT to static file (will not work) */
	conn = mg_download("localhost",
	                   ipv4_port,
	                   0,
	                   ebuf,
	                   sizeof(ebuf),
	                   "%s",
	                   "PUT /test.txt HTTP/1.0\r\n\r\n");
	ck_assert(conn != NULL);
	ri = mg_get_request_info(conn);

	ck_assert(ri != NULL);
	ck_assert_str_eq(ri->uri, "401"); /* not authorized */
	mg_close_connection(conn);

	/* TODO: Test websockets */


	/* Close the server */
	g_ctx = NULL;
	mg_stop(ctx);
	mg_Sleep(1);
}
END_TEST


Suite *make_public_server_suite(void)
{
	Suite *const suite = suite_create("PublicServer");

	TCase *const checktestenv = tcase_create("Check test environment");
	TCase *const startstophttp = tcase_create("Start Stop HTTP Server");
	TCase *const startstophttps = tcase_create("Start Stop HTTPS Server");
	TCase *const serverrequests = tcase_create("Server Requests");


	tcase_add_test(checktestenv, test_the_test_environment);
	suite_add_tcase(suite, checktestenv);

	tcase_add_test(startstophttp, test_mg_start_stop_http_server);
	suite_add_tcase(suite, startstophttp);

	tcase_add_test(startstophttps, test_mg_start_stop_https_server);
	suite_add_tcase(suite, startstophttps);

	tcase_add_test(serverrequests, test_request_handlers);
	suite_add_tcase(suite, serverrequests);

	return suite;
}


#ifdef REPLACE_CHECK_FOR_LOCAL_DEBUGGING
/* Used to debug test cases without using the check framework */

static int chk_ok = 0;
static int chk_failed = 0;

void main(void)
{
	test_the_test_environment(0);
	test_mg_start_stop_http_server(0);
	test_mg_start_stop_https_server(0);
	test_request_handlers(0);

	printf("\nok: %i\nfailed: %i\n\n", chk_ok, chk_failed);
}

void _ck_assert_failed(const char *file, int line, const char *expr, ...)
{
	va_list va;
	va_start(va, expr);
	fprintf(stderr, "Error: %s, line %i\n", file, line); /* breakpoint here ! */
	vfprintf(stderr, expr, va);
	va_end(va);
	chk_failed++;
}

void _mark_point(const char *file, int line) { chk_ok++; }

void tcase_fn_start(const char *fname, const char *file, int line) {}
void suite_add_tcase(Suite *s, TCase *tc){};
void _tcase_add_test(TCase *tc,
                     TFun tf,
                     const char *fname,
                     int _signal,
                     int allowed_exit_value,
                     int start,
                     int end){};
TCase *tcase_create(const char *name) { return NULL; };
Suite *suite_create(const char *name) { return NULL; };
#endif
