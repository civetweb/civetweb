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

#include "public_func.h"
#include <civetweb.h>

/* This unit test file uses the excellent Check unit testing library.
 * The API documentation is available here:
 * http://check.sourceforge.net/doc/check_html/index.html
 */

Suite *make_public_func_suite(void)
{
	Suite *const suite = suite_create("PublicFunc");

	TCase *const version = tcase_create("Version");
	TCase *const get_valid_options = tcase_create("Options");
	TCase *const get_builtin_mime_type = tcase_create("MIME types");
	TCase *const tstrncasecmp = tcase_create("strcasecmp");
	TCase *const urlencodingdecoding = tcase_create("URL encoding decoding");
	TCase *const cookies = tcase_create("Cookies and variables");
	TCase *const md5 = tcase_create("MD5");

	tcase_add_test(version, test_mg_version);
	suite_add_tcase(suite, version);

	tcase_add_test(get_valid_options, test_mg_get_valid_options);
	suite_add_tcase(suite, get_valid_options);

	tcase_add_test(get_builtin_mime_type, test_mg_get_builtin_mime_type);
	suite_add_tcase(suite, get_builtin_mime_type);

	tcase_add_test(tstrncasecmp, test_mg_strncasecmp);
	suite_add_tcase(suite, tstrncasecmp);

	tcase_add_test(urlencodingdecoding, test_mg_url_encode);
	tcase_add_test(urlencodingdecoding, test_mg_url_decode);
	suite_add_tcase(suite, urlencodingdecoding);

	tcase_add_test(cookies, test_mg_get_cookie);
	tcase_add_test(cookies, test_mg_get_var);
	suite_add_tcase(suite, cookies);

	tcase_add_test(md5, test_mg_md5);
	suite_add_tcase(suite, md5);

	return suite;
}
