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

#include "civetweb_check.h"
#include "shared.h"
#include "public_func.h"
#include "public_server.h"
#include "private.h"
#include "timertest.h"
#include "private_exe.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* This unit test file uses the excellent Check unit testing library.
 * The API documentation is available here:
 * http://check.sourceforge.net/doc/check_html/index.html
 */

int
main(const int argc, char *argv[])
{
	// Determine what tests to run
	const char *suite = NULL;
	const char *const suite_arg = "--suite=";
	const size_t suite_arg_size = strlen(suite_arg);
	const char *test_case = NULL;
	const char *const test_case_arg = "--test-case=";
	const size_t test_case_arg_size = strlen(test_case_arg);
	const char *const test_dir_arg = "--test-dir=";
	const size_t test_dir_arg_size = strlen(test_dir_arg);
	for (int i = 1; i < argc; ++i) {
		if (0 == strncmp(suite_arg, argv[i], suite_arg_size)
		    && (strlen(argv[i]) > suite_arg_size)) {
			suite = &argv[i][suite_arg_size];
		} else if (0 == strncmp(test_case_arg, argv[i], test_case_arg_size)
		           && (strlen(argv[i]) > test_case_arg_size)) {
			test_case = &argv[i][test_case_arg_size];
		} else if (0 == strncmp(test_dir_arg, argv[i], test_dir_arg_size)
		           && (strlen(argv[i]) > test_dir_arg_size)) {
			set_test_directory(&argv[i][test_dir_arg_size]);
		} else if (0 == strcmp("--help", argv[i])) {
			printf(
			    "Usage: %s [options]\n"
			    "  --suite=Suite            Determines the suite to run\n"
			    "  --test-case='Test Case'  Determines the test case to run\n"
			    "  --test-dir='folder/path' The location of the test directory "
			    "with the \n"
			    "                           'fixtures' and 'expected\n",
			    argv[0]);
			exit(EXIT_SUCCESS);
		} else {
			fprintf(stderr, "Invalid argument: %s\n", argv[i]);
			exit(EXIT_FAILURE);
		}
	}

	/* Run up the tests */
	SRunner *const srunner = srunner_create(make_public_func_suite());
	srunner_add_suite(srunner, make_public_server_suite());
	srunner_add_suite(srunner, make_private_suite());
	srunner_add_suite(srunner, make_private_exe_suite());
	srunner_add_suite(srunner, make_timertest_suite());

	/* Write test logs to a file */
	srunner_set_log(srunner, "test.log");
	srunner_set_xml(srunner, "test.xml");

	/* CK_NORMAL offers not enough diagnosis during setup phase*/
	srunner_run(srunner, suite, test_case, CK_VERBOSE);

	const int number_run = srunner_ntests_run(srunner);
	const int number_failed = srunner_ntests_failed(srunner);
	srunner_free(srunner);
	return (number_failed == 0) && (number_run != 0) ? EXIT_SUCCESS
	                                                 : EXIT_FAILURE;
}
