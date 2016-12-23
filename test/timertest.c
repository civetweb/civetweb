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

static int action_dec_ret;

static int
action_dec(void *arg)
{
	int *p = (int *)arg;
	(*p)--;

	if (*p < -1) {
		ck_abort_msg("Periodic timer called too often");
		return 0;
	}

	return action_dec_ret;
}


START_TEST(test_timer_cyclic)
{
	struct mg_context ctx;
	int c[10];
	memset(&ctx, 0, sizeof(ctx));
	memset(c, 0, sizeof(c));

	action_dec_ret = 1;

	mark_point();
	timers_init(&ctx);
	mg_sleep(100);
	mark_point();

	c[0] = 10;
	timer_add(&ctx, 0, 0.1, 1, action_dec, c + 0);
	c[2] = 2;
	timer_add(&ctx, 0, 0.5, 1, action_dec, c + 2);
	c[1] = 5;
	timer_add(&ctx, 0, 0.2, 1, action_dec, c + 1);

	mark_point();

	mg_sleep(1000); /* Sleep 1 second - timer will run */

	mark_point();
	ctx.stop_flag = 99; /* End timer thread */
	mark_point();

	mg_sleep(1000); /* Sleep 1 second - timer will not run */

	mark_point();

	timers_exit(&ctx);

	mark_point();
	mg_sleep(100);

#ifdef LOCAL_TEST
	/* If performed locally (on a physical machine that's not overloaded),
	 * timing will be precise to the ~100 ms required here. */
	ck_assert_int_eq(c[0], 0);
	ck_assert_int_eq(c[1], 0);
	ck_assert_int_eq(c[2], 0);
#else
	/* If this test runs in a virtual environment, like the CI unit test
	 * containers, there might be some timing deviations, so check the
	 * counter with some tolerance. */
	ck_assert_int_ge(c[0], -1);
	ck_assert_int_le(c[0], +1);
	ck_assert_int_ge(c[1], -1);
	ck_assert_int_le(c[1], +1);
	ck_assert_int_ge(c[2], -1);
	ck_assert_int_le(c[2], +1);
#endif
}
END_TEST


START_TEST(test_timer_oneshot_by_callback_retval)
{
	struct mg_context ctx;
	int c[10];
	memset(&ctx, 0, sizeof(ctx));
	memset(c, 0, sizeof(c));

	action_dec_ret = 0;

	mark_point();
	timers_init(&ctx);
	mg_sleep(100);
	mark_point();

	c[0] = 10;
	timer_add(&ctx, 0, 0.1, 1, action_dec, c + 0);
	c[2] = 2;
	timer_add(&ctx, 0, 0.5, 1, action_dec, c + 2);
	c[1] = 5;
	timer_add(&ctx, 0, 0.2, 1, action_dec, c + 1);

	mark_point();

	mg_sleep(1000); /* Sleep 1 second - timer will run */

	mark_point();
	ctx.stop_flag = 99; /* End timer thread */
	mark_point();

	mg_sleep(1000); /* Sleep 1 second - timer will not run */

	mark_point();

	timers_exit(&ctx);

	mark_point();
	mg_sleep(100);

	ck_assert_int_eq(c[0], 9);
	ck_assert_int_eq(c[1], 4);
	ck_assert_int_eq(c[2], 1);
}
END_TEST


START_TEST(test_timer_oneshot_by_timer_add)
{
	struct mg_context ctx;
	int c[10];
	memset(&ctx, 0, sizeof(ctx));
	memset(c, 0, sizeof(c));

	action_dec_ret = 1;

	mark_point();
	timers_init(&ctx);
	mg_sleep(100);
	mark_point();

	c[0] = 10;
	timer_add(&ctx, 0, 0, 1, action_dec, c + 0);
	c[2] = 2;
	timer_add(&ctx, 0, 0, 1, action_dec, c + 2);
	c[1] = 5;
	timer_add(&ctx, 0, 0, 1, action_dec, c + 1);

	mark_point();

	mg_sleep(1000); /* Sleep 1 second - timer will run */

	mark_point();
	ctx.stop_flag = 99; /* End timer thread */
	mark_point();

	mg_sleep(1000); /* Sleep 1 second - timer will not run */

	mark_point();

	timers_exit(&ctx);

	mark_point();
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

	TCase *const tcase_timer_cyclic = tcase_create("Timer Periodic");
	TCase *const tcase_timer_oneshot = tcase_create("Timer Single Shot");

	tcase_add_test(tcase_timer_cyclic, test_timer_cyclic);
	tcase_set_timeout(tcase_timer_cyclic, 30);
	suite_add_tcase(suite, tcase_timer_cyclic);

	tcase_add_test(tcase_timer_oneshot, test_timer_oneshot_by_timer_add);
	tcase_add_test(tcase_timer_oneshot, test_timer_oneshot_by_callback_retval);
	tcase_set_timeout(tcase_timer_oneshot, 30);
	suite_add_tcase(suite, tcase_timer_oneshot);

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

	test_timer_cyclic(0);
	test_timer_oneshot_by_timer_add(0);
	test_timer_oneshot_by_callback_retval(0);

#if defined(_WIN32)
	WSACleanup();
#endif
}

#endif
