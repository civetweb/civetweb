/* Copyright (c) 2016 the Civetweb developers
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
#endif

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

#define CIVETWEB_API static
#define USE_TIMERS

#include "../src/civetweb.c"

#include <stdlib.h>
#include <time.h>

#include "timertest.h"


static int
action1(void *arg)
{
	int *p = (int *)arg;
	(*p)--;

	ck_assert_int_ge(*p, -1);

	return 1;
}


START_TEST(test_timer1)
{
	struct mg_context ctx;
	int c[10];
	memset(&ctx, 0, sizeof(ctx));
	memset(c, 0, sizeof(c));

	mark_point();
	timers_init(&ctx);
	mg_sleep(100);
	mark_point();

	c[0] = 10;
	timer_add(&ctx, 0, 0.1, 1, action1, c + 0);
	c[2] = 2;
	timer_add(&ctx, 0, 0.5, 1, action1, c + 2);
	c[1] = 5;
	timer_add(&ctx, 0, 0.2, 1, action1, c + 1);

	mg_sleep(1000);

	ctx.stop_flag = 99;
	mg_sleep(100);
	timers_exit(&ctx);
	mg_sleep(100);

#ifdef LOCAL_TEST
	ck_assert_int_eq(c[0], 0);
	ck_assert_int_eq(c[1], 0);
	ck_assert_int_eq(c[2], 0);
#else
	ck_assert_int_ge(c[0], -1);
	ck_assert_int_le(c[0], +1);
	ck_assert_int_ge(c[1], -1);
	ck_assert_int_le(c[1], +1);
	ck_assert_int_ge(c[2], -1);
	ck_assert_int_le(c[2], +1);
#endif
}
END_TEST


static int
action2(void *arg)
{
	int *p = (int *)arg;
	(*p)--;

	ck_assert_int_ge(*p, -1);

	return 0;
}


START_TEST(test_timer2)
{
	struct mg_context ctx;
	int c[10];
	memset(&ctx, 0, sizeof(ctx));
	memset(c, 0, sizeof(c));

	mark_point();
	timers_init(&ctx);
	mg_sleep(100);
	mark_point();

	c[0] = 10;
	timer_add(&ctx, 0, 0.1, 1, action2, c + 0);
	c[2] = 2;
	timer_add(&ctx, 0, 0.5, 1, action2, c + 2);
	c[1] = 5;
	timer_add(&ctx, 0, 0.2, 1, action2, c + 1);

	mg_sleep(1000);

	ctx.stop_flag = 99;
	mg_sleep(100);
	timers_exit(&ctx);
	mg_sleep(100);

	ck_assert_int_eq(c[0], 9);
	ck_assert_int_eq(c[1], 4);
	ck_assert_int_eq(c[2], 1);
}
END_TEST


Suite *
make_timertest_suite(void)
{
	Suite *const suite = suite_create("Timer");

	TCase *const tcase_timer1 = tcase_create("Timer Periodic");
	TCase *const tcase_timer2 = tcase_create("Timer Single Shot");

	tcase_add_test(tcase_timer1, test_timer1);
	tcase_set_timeout(tcase_timer1, 30);
	suite_add_tcase(suite, tcase_timer1);

	tcase_add_test(tcase_timer2, test_timer2);
	tcase_set_timeout(tcase_timer2, 30);
	suite_add_tcase(suite, tcase_timer2);

	return suite;
}


#ifdef REPLACE_CHECK_FOR_LOCAL_DEBUGGING
/* Used to debug test cases without using the check framework */

void
TIMER_PRIVATE(void)
{
#if defined(_WIN32)
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);
#endif

	test_timer1(0);
	test_timer2(0);

#if defined(_WIN32)
	WSACleanup();
#endif
}

#endif
