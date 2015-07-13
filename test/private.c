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

/**
 * We include the source file so that we access to the internal private
 * static functions
 */
#include "../src/civetweb.c"

#include <stdlib.h>

#include "private.h"

/* This unit test file uses the excellent Check unit testing library.
 * The API documentation is available here:
 * http://check.sourceforge.net/doc/check_html/index.html
 */

START_TEST (test_parse_http_message)
{
  struct mg_request_info ri;
  char req1[] = "GET / HTTP/1.1\r\n\r\n";
  char req2[] = "BLAH / HTTP/1.1\r\n\r\n";
  char req3[] = "GET / HTTP/1.1\r\nBah\r\n";
  char req4[] = "GET / HTTP/1.1\r\nA: foo bar\r\nB: bar\r\nbaz\r\n\r\n";
  char req5[] = "GET / HTTP/1.1\r\n\r\n";
  char req6[] = "G";
  char req7[] = " blah ";
  char req8[] = " HTTP/1.1 200 OK \n\n";
  char req9[] = "HTTP/1.1 200 OK\r\nConnection: close\r\n\r\n";

  ck_assert_int_eq(sizeof(req9) - 1, parse_http_message(req9, sizeof(req9), &ri));
  ck_assert_int_eq(1, ri.num_headers);

  ck_assert_int_eq(sizeof(req1) - 1, parse_http_message(req1, sizeof(req1), &ri));
  ck_assert_str_eq("1.1", ri.http_version);
  ck_assert_int_eq(0, ri.num_headers);

  ck_assert_int_eq(-1, parse_http_message(req2, sizeof(req2), &ri));
  ck_assert_int_eq(0, parse_http_message(req3, sizeof(req3), &ri));
  ck_assert_int_eq(0, parse_http_message(req6, sizeof(req6), &ri));
  ck_assert_int_eq(0, parse_http_message(req7, sizeof(req7), &ri));
  ck_assert_int_eq(0, parse_http_message("", 0, &ri));
  ck_assert_int_eq(sizeof(req8) - 1, parse_http_message(req8, sizeof(req8), &ri));

  /* TODO(lsm): Fix this. Header value may span multiple lines. */
  ck_assert_int_eq(sizeof(req4) - 1, parse_http_message(req4, sizeof(req4), &ri));
  ck_assert_str_eq("1.1", ri.http_version);
  ck_assert_int_eq(3, ri.num_headers);
  ck_assert_str_eq("A", ri.http_headers[0].name);
  ck_assert_str_eq("foo bar", ri.http_headers[0].value);
  ck_assert_str_eq("B", ri.http_headers[1].name);
  ck_assert_str_eq("bar", ri.http_headers[1].value);
  ck_assert_str_eq("baz\r\n\r", ri.http_headers[2].name);
  ck_assert_str_eq("", ri.http_headers[2].value);

  ck_assert_int_eq(sizeof(req5) - 1, parse_http_message(req5, sizeof(req5), &ri));
  ck_assert_str_eq("GET", ri.request_method);
  ck_assert_str_eq("1.1", ri.http_version);
}
END_TEST

Suite * make_private_suite (void) {
  Suite * const suite = suite_create("Private");

  TCase * const http_message = tcase_create("HTTP Message");
  tcase_add_test(http_message, test_parse_http_message);
  suite_add_tcase(suite, http_message);

  return suite;
}
