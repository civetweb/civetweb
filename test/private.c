/* Copyright (c) 2015-2016 the Civetweb developers
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

/**
 * We include the source file so that we have access to the internal private
 * static functions
 */
#ifdef _MSC_VER
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#define CIVETWEB_API static
#endif

#ifdef REPLACE_CHECK_FOR_LOCAL_DEBUGGING
#define HAVE_STDINT
#undef MEMORY_DEBUGGING
#endif

#include "../src/civetweb.c"

#include <stdlib.h>

#include "private.h"

/* This unit test file uses the excellent Check unit testing library.
 * The API documentation is available here:
 * http://check.sourceforge.net/doc/check_html/index.html
 */


START_TEST(test_parse_http_message)
{
	/* Adapted from unit_test.c */
	/* Copyright (c) 2013-2015 the Civetweb developers */
	/* Copyright (c) 2004-2013 Sergey Lyubka */
	struct mg_request_info ri;
	char empty[] = "";
	char req1[] = "GET / HTTP/1.1\r\n\r\n";
	char req2[] = "BLAH / HTTP/1.1\r\n\r\n";
	char req3[] = "GET / HTTP/1.1\r\nBah\r\n";
	char req4[] = "GET / HTTP/1.1\r\nA: foo bar\r\nB: bar\r\nbaz\r\n\r\n";
	char req5[] = "GET / HTTP/1.1\r\n\r\n";
	char req6[] = "G";
	char req7[] = " blah ";
	char req8[] = " HTTP/1.1 200 OK \n\n";
	char req9[] = "HTTP/1.1 200 OK\r\nConnection: close\r\n\r\n";

	ck_assert_int_eq(sizeof(req9) - 1,
	                 parse_http_message(req9, sizeof(req9), &ri));
	ck_assert_int_eq(1, ri.num_headers);

	ck_assert_int_eq(sizeof(req1) - 1,
	                 parse_http_message(req1, sizeof(req1), &ri));
	ck_assert_str_eq("1.1", ri.http_version);
	ck_assert_int_eq(0, ri.num_headers);

	ck_assert_int_eq(-1, parse_http_message(req2, sizeof(req2), &ri));
	ck_assert_int_eq(0, parse_http_message(req3, sizeof(req3), &ri));
	ck_assert_int_eq(0, parse_http_message(req6, sizeof(req6), &ri));
	ck_assert_int_eq(0, parse_http_message(req7, sizeof(req7), &ri));
	ck_assert_int_eq(0, parse_http_message(empty, 0, &ri));
	ck_assert_int_eq(sizeof(req8) - 1,
	                 parse_http_message(req8, sizeof(req8), &ri));

	/* TODO(lsm): Fix this. Header value may span multiple lines. */
	ck_assert_int_eq(sizeof(req4) - 1,
	                 parse_http_message(req4, sizeof(req4), &ri));
	ck_assert_str_eq("1.1", ri.http_version);
	ck_assert_int_eq(3, ri.num_headers);
	ck_assert_str_eq("A", ri.http_headers[0].name);
	ck_assert_str_eq("foo bar", ri.http_headers[0].value);
	ck_assert_str_eq("B", ri.http_headers[1].name);
	ck_assert_str_eq("bar", ri.http_headers[1].value);
	ck_assert_str_eq("baz", ri.http_headers[2].name);
	ck_assert(ri.http_headers[2].value == NULL);

	ck_assert_int_eq(sizeof(req5) - 1,
	                 parse_http_message(req5, sizeof(req5), &ri));
	ck_assert_str_eq("GET", ri.request_method);
	ck_assert_str_eq("1.1", ri.http_version);
}
END_TEST


START_TEST(test_should_keep_alive)
{
	/* Adapted from unit_test.c */
	/* Copyright (c) 2013-2015 the Civetweb developers */
	/* Copyright (c) 2004-2013 Sergey Lyubka */
	struct mg_connection conn;
	struct mg_context ctx;
	char req1[] = "GET / HTTP/1.1\r\n\r\n";
	char req2[] = "GET / HTTP/1.0\r\n\r\n";
	char req3[] = "GET / HTTP/1.1\r\nConnection: close\r\n\r\n";
	char req4[] = "GET / HTTP/1.1\r\nConnection: keep-alive\r\n\r\n";
	char yes[] = "yes";
	char no[] = "no";

	memset(&conn, 0, sizeof(conn));
	conn.ctx = &ctx;
	ck_assert_int_eq(parse_http_message(req1, sizeof(req1), &conn.request_info),
	                 sizeof(req1) - 1);

	ctx.config[ENABLE_KEEP_ALIVE] = no;
	ck_assert_int_eq(should_keep_alive(&conn), 0);

	ctx.config[ENABLE_KEEP_ALIVE] = yes;
	ck_assert_int_eq(should_keep_alive(&conn), 1);

	conn.must_close = 1;
	ck_assert_int_eq(should_keep_alive(&conn), 0);

	conn.must_close = 0;
	parse_http_message(req2, sizeof(req2), &conn.request_info);
	ck_assert_int_eq(should_keep_alive(&conn), 0);

	parse_http_message(req3, sizeof(req3), &conn.request_info);
	ck_assert_int_eq(should_keep_alive(&conn), 0);

	parse_http_message(req4, sizeof(req4), &conn.request_info);
	ck_assert_int_eq(should_keep_alive(&conn), 1);

	conn.status_code = 401;
	ck_assert_int_eq(should_keep_alive(&conn), 0);

	conn.status_code = 200;
	conn.must_close = 1;
	ck_assert_int_eq(should_keep_alive(&conn), 0);
}
END_TEST


START_TEST(test_match_prefix)
{
	/* Adapted from unit_test.c */
	/* Copyright (c) 2013-2015 the Civetweb developers */
	/* Copyright (c) 2004-2013 Sergey Lyubka */
	ck_assert_int_eq(4, match_prefix("/api", 4, "/api"));
	ck_assert_int_eq(3, match_prefix("/a/", 3, "/a/b/c"));
	ck_assert_int_eq(-1, match_prefix("/a/", 3, "/ab/c"));
	ck_assert_int_eq(4, match_prefix("/*/", 3, "/ab/c"));
	ck_assert_int_eq(6, match_prefix("**", 2, "/a/b/c"));
	ck_assert_int_eq(2, match_prefix("/*", 2, "/a/b/c"));
	ck_assert_int_eq(2, match_prefix("*/*", 3, "/a/b/c"));
	ck_assert_int_eq(5, match_prefix("**/", 3, "/a/b/c"));
	ck_assert_int_eq(5, match_prefix("**.foo|**.bar", 13, "a.bar"));
	ck_assert_int_eq(2, match_prefix("a|b|cd", 6, "cdef"));
	ck_assert_int_eq(2, match_prefix("a|b|c?", 6, "cdef"));
	ck_assert_int_eq(1, match_prefix("a|?|cd", 6, "cdef"));
	ck_assert_int_eq(-1, match_prefix("/a/**.cgi", 9, "/foo/bar/x.cgi"));
	ck_assert_int_eq(12, match_prefix("/a/**.cgi", 9, "/a/bar/x.cgi"));
	ck_assert_int_eq(5, match_prefix("**/", 3, "/a/b/c"));
	ck_assert_int_eq(-1, match_prefix("**/$", 4, "/a/b/c"));
	ck_assert_int_eq(5, match_prefix("**/$", 4, "/a/b/"));
	ck_assert_int_eq(0, match_prefix("$", 1, ""));
	ck_assert_int_eq(-1, match_prefix("$", 1, "x"));
	ck_assert_int_eq(1, match_prefix("*$", 2, "x"));
	ck_assert_int_eq(1, match_prefix("/$", 2, "/"));
	ck_assert_int_eq(-1, match_prefix("**/$", 4, "/a/b/c"));
	ck_assert_int_eq(5, match_prefix("**/$", 4, "/a/b/"));
	ck_assert_int_eq(0, match_prefix("*", 1, "/hello/"));
	ck_assert_int_eq(-1, match_prefix("**.a$|**.b$", 11, "/a/b.b/"));
	ck_assert_int_eq(6, match_prefix("**.a$|**.b$", 11, "/a/b.b"));
	ck_assert_int_eq(6, match_prefix("**.a$|**.b$", 11, "/a/B.A"));
	ck_assert_int_eq(5, match_prefix("**o$", 4, "HELLO"));
}
END_TEST


START_TEST(test_remove_double_dots_and_double_slashes)
{
	/* Adapted from unit_test.c */
	/* Copyright (c) 2013-2015 the Civetweb developers */
	/* Copyright (c) 2004-2013 Sergey Lyubka */
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
		ck_assert_str_eq(data[i].before, data[i].after);
	}
}
END_TEST


START_TEST(test_is_valid_uri)
{
	/* is_valid_uri is superseeded by get_uri_type */
	ck_assert_int_eq(2, get_uri_type("/api"));
	ck_assert_int_eq(2, get_uri_type("/api/"));
	ck_assert_int_eq(2,
	                 get_uri_type("/some/long/path%20with%20space/file.xyz"));
	ck_assert_int_eq(0, get_uri_type("api"));
	ck_assert_int_eq(1, get_uri_type("*"));
	ck_assert_int_eq(0, get_uri_type("*xy"));
	ck_assert_int_eq(3, get_uri_type("http://somewhere/"));
	ck_assert_int_eq(3, get_uri_type("https://somewhere/some/file.html"));
	ck_assert_int_eq(4, get_uri_type("http://somewhere:8080/"));
	ck_assert_int_eq(4, get_uri_type("https://somewhere:8080/some/file.html"));
}
END_TEST


START_TEST(test_next_option)
{
	/* Adapted from unit_test.c */
	/* Copyright (c) 2013-2015 the Civetweb developers */
	/* Copyright (c) 2004-2013 Sergey Lyubka */
	const char *p, *list = "x/8,/y**=1;2k,z";
	struct vec a, b;
	int i;

	ck_assert(next_option(NULL, &a, &b) == NULL);
	for (i = 0, p = list; (p = next_option(p, &a, &b)) != NULL; i++) {
		ck_assert(i != 0 || (a.ptr == list && a.len == 3 && b.len == 0));
		ck_assert(i != 1
		          || (a.ptr == list + 4 && a.len == 4 && b.ptr == list + 9
		              && b.len == 4));
		ck_assert(i != 2 || (a.ptr == list + 14 && a.len == 1 && b.len == 0));
	}
}
END_TEST


START_TEST(test_skip_quoted)
{
	/* Adapted from unit_test.c */
	/* Copyright (c) 2013-2015 the Civetweb developers */
	/* Copyright (c) 2004-2013 Sergey Lyubka */
	char x[] = "a=1, b=2, c='hi \' there', d='here\\, there'", *s = x, *p;

	p = skip_quoted(&s, ", ", ", ", 0);
	ck_assert(p != NULL && !strcmp(p, "a=1"));

	p = skip_quoted(&s, ", ", ", ", 0);
	ck_assert(p != NULL && !strcmp(p, "b=2"));

	p = skip_quoted(&s, ",", " ", 0);
	ck_assert(p != NULL && !strcmp(p, "c='hi \' there'"));

	p = skip_quoted(&s, ",", " ", '\\');
	ck_assert(p != NULL && !strcmp(p, "d='here, there'"));
	ck_assert(*s == 0);
}
END_TEST


static int
alloc_printf(char **buf, size_t size, const char *fmt, ...)
{
	/* Test helper function - adapted from unit_test.c */
	/* Copyright (c) 2013-2015 the Civetweb developers */
	/* Copyright (c) 2004-2013 Sergey Lyubka */
	va_list ap;
	int ret = 0;
	va_start(ap, fmt);
	ret = alloc_vprintf(buf, *buf, size, fmt, ap);
	va_end(ap);
	return ret;
}


static int
alloc_printf2(char **buf, const char *fmt, ...)
{
	/* Test alternative implementation */
	va_list ap;
	int ret = 0;
	va_start(ap, fmt);
	ret = alloc_vprintf2(buf, fmt, ap);
	va_end(ap);
	return ret;
}


START_TEST(test_alloc_vprintf)
{
	/* Adapted from unit_test.c */
	/* Copyright (c) 2013-2015 the Civetweb developers */
	/* Copyright (c) 2004-2013 Sergey Lyubka */
	char buf[MG_BUF_LEN], *p = buf;

	ck_assert(alloc_printf(&p, sizeof(buf), "%s", "hi") == 2);
	ck_assert(p == buf);

	ck_assert(alloc_printf(&p, sizeof(buf), "%s", "") == 0);
	ck_assert(p == buf);

	ck_assert(alloc_printf(&p, sizeof(buf), "") == 0);
	ck_assert(p == buf);

	/* Pass small buffer, make sure alloc_printf allocates */
	ck_assert(alloc_printf(&p, 1, "%s", "hello") == 5);
	ck_assert(p != buf);
	mg_free(p);
	p = buf;

	/* Test alternative implementation */
	ck_assert(alloc_printf2(&p, "%s", "hello") == 5);
	ck_assert(p != buf);
	mg_free(p);
	p = buf;
}
END_TEST


START_TEST(test_mg_vsnprintf)
{
	char buf[16];
	int trunc;

	memset(buf, 0, sizeof(buf));

	trunc = 777;
	mg_snprintf(NULL, &trunc, buf, 10, "%8i", 123);
	ck_assert_str_eq(buf, "     123");
	ck_assert_int_eq(trunc, 0);

	trunc = 777;
	mg_snprintf(NULL, &trunc, buf, 10, "%9i", 123);
	ck_assert_str_eq(buf, "      123");
	ck_assert_int_eq(trunc, 0);

	trunc = 777;
	mg_snprintf(NULL, &trunc, buf, 9, "%9i", 123);
	ck_assert_str_eq(buf, "      12");
	ck_assert_int_eq(trunc, 1);

	trunc = 777;
	mg_snprintf(NULL, &trunc, buf, 8, "%9i", 123);
	ck_assert_str_eq(buf, "      1");
	ck_assert_int_eq(trunc, 1);

	trunc = 777;
	mg_snprintf(NULL, &trunc, buf, 7, "%9i", 123);
	ck_assert_str_eq(buf, "      ");
	ck_assert_int_eq(trunc, 1);

	strcpy(buf, "1234567890");
	mg_snprintf(NULL, &trunc, buf, 0, "%i", 543);
	ck_assert_str_eq(buf, "1234567890");
}
END_TEST


START_TEST(test_mg_strcasestr)
{
	/* Adapted from unit_test.c */
	/* Copyright (c) 2013-2015 the Civetweb developers */
	/* Copyright (c) 2004-2013 Sergey Lyubka */
	static const char *big1 = "abcdef";
	ck_assert(mg_strcasestr("Y", "X") == NULL);
	ck_assert(mg_strcasestr("Y", "y") != NULL);
	ck_assert(mg_strcasestr(big1, "X") == NULL);
	ck_assert(mg_strcasestr(big1, "CD") == big1 + 2);
	ck_assert(mg_strcasestr("aa", "AAB") == NULL);
}
END_TEST


START_TEST(test_parse_port_string)
{
	/* Adapted from unit_test.c */
	/* Copyright (c) 2013-2015 the Civetweb developers */
	/* Copyright (c) 2004-2013 Sergey Lyubka */
	static const char *valid[] =
	{ "0",
	  "1",
	  "1s",
	  "1r",
	  "1.2.3.4:1",
	  "1.2.3.4:1s",
	  "1.2.3.4:1r",
#if defined(USE_IPV6)
	  "[::1]:123",
	  "[::]:80",
	  "[3ffe:2a00:100:7031::1]:900",
#endif
	  NULL };
	static const char *invalid[] = {
	    "99999", "1k", "1.2.3", "1.2.3.4:", "1.2.3.4:2p", NULL};
	struct socket so;
	struct vec vec;
	int i;

	for (i = 0; valid[i] != NULL; i++) {
		vec.ptr = valid[i];
		vec.len = strlen(vec.ptr);
		ck_assert(parse_port_string(&vec, &so) != 0);
	}

	for (i = 0; invalid[i] != NULL; i++) {
		vec.ptr = invalid[i];
		vec.len = strlen(vec.ptr);
		ck_assert(parse_port_string(&vec, &so) == 0);
	}
}
END_TEST


START_TEST(test_encode_decode)
{
	char buf[128];
	const char *alpha = "abcdefghijklmnopqrstuvwxyz";
	const char *nonalpha = " !\"#$%&'()*+,-./0123456789:;<=>?@";
	const char *nonalpha_url_enc1 =
	    "%20%21%22%23$%25%26%27()%2a%2b,-.%2f0123456789%3a;%3c%3d%3e%3f%40";
	const char *nonalpha_url_enc2 =
	    "%20!%22%23%24%25%26'()*%2B%2C-.%2F0123456789%3A%3B%3C%3D%3E%3F%40";
	int ret;
	size_t len;

#if defined(USE_WEBSOCKET) || defined(USE_LUA)
	const char *alpha_b64_enc = "YWJjZGVmZ2hpamtsbW5vcHFyc3R1dnd4eXo=";
	const char *nonalpha_b64_enc =
	    "ICEiIyQlJicoKSorLC0uLzAxMjM0NTY3ODk6Ozw9Pj9A";

	memset(buf, 77, sizeof(buf));
	base64_encode((unsigned char *)"a", 1, buf);
	ck_assert_str_eq(buf, "YQ==");

	memset(buf, 77, sizeof(buf));
	base64_encode((unsigned char *)"ab", 1, buf);
	ck_assert_str_eq(buf, "YQ==");

	memset(buf, 77, sizeof(buf));
	base64_encode((unsigned char *)"ab", 2, buf);
	ck_assert_str_eq(buf, "YWI=");

	memset(buf, 77, sizeof(buf));
	base64_encode((unsigned char *)alpha, 3, buf);
	ck_assert_str_eq(buf, "YWJj");

	memset(buf, 77, sizeof(buf));
	base64_encode((unsigned char *)alpha, 4, buf);
	ck_assert_str_eq(buf, "YWJjZA==");

	memset(buf, 77, sizeof(buf));
	base64_encode((unsigned char *)alpha, 5, buf);
	ck_assert_str_eq(buf, "YWJjZGU=");

	memset(buf, 77, sizeof(buf));
	base64_encode((unsigned char *)alpha, 6, buf);
	ck_assert_str_eq(buf, "YWJjZGVm");

	memset(buf, 77, sizeof(buf));
	base64_encode((unsigned char *)alpha, (int)strlen(alpha), buf);
	ck_assert_str_eq(buf, alpha_b64_enc);

	memset(buf, 77, sizeof(buf));
	base64_encode((unsigned char *)nonalpha, (int)strlen(nonalpha), buf);
	ck_assert_str_eq(buf, nonalpha_b64_enc);
#endif

#if defined(USE_LUA)
	memset(buf, 77, sizeof(buf));
	len = 9999;
	ret = base64_decode((unsigned char *)alpha_b64_enc,
	                    (int)strlen(alpha_b64_enc),
	                    buf,
	                    &len);
	ck_assert_int_eq(ret, -1);
	ck_assert_uint_eq((unsigned int)len, (unsigned int)strlen(alpha));
	ck_assert_str_eq(buf, alpha);

	memset(buf, 77, sizeof(buf));
	len = 9999;
	ret = base64_decode((unsigned char *)"AAA*AAA", 7, buf, &len);
	ck_assert_int_eq(ret, 3);
#endif

	memset(buf, 77, sizeof(buf));
	ret = mg_url_encode(alpha, buf, sizeof(buf));
	ck_assert_int_eq(ret, (int)strlen(buf));
	ck_assert_int_eq(ret, (int)strlen(alpha));
	ck_assert_str_eq(buf, alpha);

	memset(buf, 77, sizeof(buf));
	ret = mg_url_encode(nonalpha, buf, sizeof(buf));
	ck_assert_int_eq(ret, (int)strlen(buf));
	ck_assert_int_eq(ret, (int)strlen(nonalpha_url_enc1));
	ck_assert_str_eq(buf, nonalpha_url_enc1);

	memset(buf, 77, sizeof(buf));
	ret = mg_url_decode(alpha, (int)strlen(alpha), buf, sizeof(buf), 0);
	ck_assert_int_eq(ret, (int)strlen(buf));
	ck_assert_int_eq(ret, (int)strlen(alpha));
	ck_assert_str_eq(buf, alpha);

	memset(buf, 77, sizeof(buf));
	ret = mg_url_decode(
	    nonalpha_url_enc1, (int)strlen(nonalpha_url_enc1), buf, sizeof(buf), 0);
	ck_assert_int_eq(ret, (int)strlen(buf));
	ck_assert_int_eq(ret, (int)strlen(nonalpha));
	ck_assert_str_eq(buf, nonalpha);

	memset(buf, 77, sizeof(buf));
	ret = mg_url_decode(
	    nonalpha_url_enc2, (int)strlen(nonalpha_url_enc2), buf, sizeof(buf), 0);
	ck_assert_int_eq(ret, (int)strlen(buf));
	ck_assert_int_eq(ret, (int)strlen(nonalpha));
	ck_assert_str_eq(buf, nonalpha);

	/* len could be unused, if base64_decode is not tested because USE_LUA is
	 * not defined */
	(void)len;
}
END_TEST


START_TEST(test_mask_data)
{
#if defined(USE_WEBSOCKET)
	char in[1024];
	char out[1024];
	int i;
#endif

	uint32_t mask = 0x61626364;
	/* TODO: adapt test for big endian */
	ck_assert((*(unsigned char *)&mask) == 0x64u);

#if defined(USE_WEBSOCKET)
	memset(in, 0, sizeof(in));
	memset(out, 99, sizeof(out));

	mask_data(in, sizeof(out), 0, out);
	ck_assert(!memcmp(out, in, sizeof(out)));

	for (i = 0; i < 1024; i++) {
		in[i] = (char)((unsigned char)i);
	}
	mask_data(in, 107, 0, out);
	ck_assert(!memcmp(out, in, 107));

	mask_data(in, 256, 0x01010101, out);
	for (i = 0; i < 256; i++) {
		ck_assert_int_eq((int)((unsigned char)out[i]),
		                 (int)(((unsigned char)in[i]) ^ (char)1u));
	}
	for (i = 256; i < (int)sizeof(out); i++) {
		ck_assert_int_eq((int)((unsigned char)out[i]), (int)0);
	}

	/* TODO: check this for big endian */
	mask_data(in, 5, 0x01020304, out);
	ck_assert_uint_eq((unsigned char)out[0], 0u ^ 4u);
	ck_assert_uint_eq((unsigned char)out[1], 1u ^ 3u);
	ck_assert_uint_eq((unsigned char)out[2], 2u ^ 2u);
	ck_assert_uint_eq((unsigned char)out[3], 3u ^ 1u);
	ck_assert_uint_eq((unsigned char)out[4], 4u ^ 4u);
#endif
}
END_TEST


START_TEST(test_parse_date_string)
{
#if !defined(NO_CACHING)
	time_t now = time(0);
	struct tm *tm = gmtime(&now);
	char date[64] = {0};
	unsigned long i;

	ck_assert_uint_eq((unsigned long)parse_date_string("1/Jan/1970 00:01:02"),
	                  62ul);
	ck_assert_uint_eq((unsigned long)parse_date_string("1 Jan 1970 00:02:03"),
	                  123ul);
	ck_assert_uint_eq((unsigned long)parse_date_string("1-Jan-1970 00:03:04"),
	                  184ul);
	ck_assert_uint_eq((unsigned long)parse_date_string(
	                      "Xyz, 1 Jan 1970 00:04:05"),
	                  245ul);

	gmt_time_string(date, sizeof(date), &now);
	ck_assert_uint_eq((uintmax_t)parse_date_string(date), (uintmax_t)now);

	sprintf(date,
	        "%02u %s %04u %02u:%02u:%02u",
	        tm->tm_mday,
	        month_names[tm->tm_mon],
	        tm->tm_year + 1900,
	        tm->tm_hour,
	        tm->tm_min,
	        tm->tm_sec);
	ck_assert_uint_eq((uintmax_t)parse_date_string(date), (uintmax_t)now);

	gmt_time_string(date, 1, NULL);
	ck_assert_str_eq(date, "");
	gmt_time_string(date, 6, NULL);
	ck_assert_str_eq(date,
	                 "Thu, "); /* part of "Thu, 01 Jan 1970 00:00:00 GMT" */
	gmt_time_string(date, sizeof(date), NULL);
	ck_assert_str_eq(date, "Thu, 01 Jan 1970 00:00:00 GMT");

	for (i = 2ul; i < 0x8000000ul; i += i / 2) {
		now = (time_t)i;

		gmt_time_string(date, sizeof(date), &now);
		ck_assert_uint_eq((uintmax_t)parse_date_string(date), (uintmax_t)now);

		tm = gmtime(&now);
		sprintf(date,
		        "%02u-%s-%04u %02u:%02u:%02u",
		        tm->tm_mday,
		        month_names[tm->tm_mon],
		        tm->tm_year + 1900,
		        tm->tm_hour,
		        tm->tm_min,
		        tm->tm_sec);
		ck_assert_uint_eq((uintmax_t)parse_date_string(date), (uintmax_t)now);
	}
#endif
}
END_TEST


Suite *
make_private_suite(void)
{
	Suite *const suite = suite_create("Private");

	TCase *const tcase_http_message = tcase_create("HTTP Message");
	TCase *const tcase_url_parsing = tcase_create("URL Parsing");
	TCase *const tcase_internal_parse = tcase_create("Internal Parsing");
	TCase *const tcase_encode_decode = tcase_create("Encode Decode");
	TCase *const tcase_mask_data = tcase_create("Mask Data");
	TCase *const tcase_parse_date_string = tcase_create("Date Parsing");

	tcase_add_test(tcase_http_message, test_parse_http_message);
	tcase_add_test(tcase_http_message, test_should_keep_alive);
	tcase_set_timeout(tcase_http_message, civetweb_min_test_timeout);
	suite_add_tcase(suite, tcase_http_message);

	tcase_add_test(tcase_url_parsing, test_match_prefix);
	tcase_add_test(tcase_url_parsing,
	               test_remove_double_dots_and_double_slashes);
	tcase_add_test(tcase_url_parsing, test_is_valid_uri);
	tcase_set_timeout(tcase_url_parsing, civetweb_min_test_timeout);
	suite_add_tcase(suite, tcase_url_parsing);

	tcase_add_test(tcase_internal_parse, test_next_option);
	tcase_add_test(tcase_internal_parse, test_skip_quoted);
	tcase_add_test(tcase_internal_parse, test_mg_strcasestr);
	tcase_add_test(tcase_internal_parse, test_alloc_vprintf);
	tcase_add_test(tcase_internal_parse, test_mg_vsnprintf);
	tcase_add_test(tcase_internal_parse, test_parse_port_string);
	tcase_set_timeout(tcase_internal_parse, civetweb_min_test_timeout);
	suite_add_tcase(suite, tcase_internal_parse);

	tcase_add_test(tcase_encode_decode, test_encode_decode);
	tcase_set_timeout(tcase_encode_decode, civetweb_min_test_timeout);
	suite_add_tcase(suite, tcase_encode_decode);

	tcase_add_test(tcase_mask_data, test_mask_data);
	tcase_set_timeout(tcase_mask_data, civetweb_min_test_timeout);
	suite_add_tcase(suite, tcase_mask_data);

	tcase_add_test(tcase_parse_date_string, test_parse_date_string);
	tcase_set_timeout(tcase_mask_data, civetweb_min_test_timeout);
	suite_add_tcase(suite, tcase_parse_date_string);

	return suite;
}


#ifdef REPLACE_CHECK_FOR_LOCAL_DEBUGGING
/* Used to debug test cases without using the check framework */

void
MAIN_PRIVATE(void)
{
	test_alloc_vprintf(0);
	test_mg_vsnprintf(0);
	test_parse_date_string(0);
}

#endif
