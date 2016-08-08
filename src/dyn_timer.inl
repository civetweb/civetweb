typedef int (*taction)(void *arg);

struct ttimer {
	double         time;
	double         period;
	taction        action;
	void          *arg;
  struct ttimer *next;
};

struct ttimers {
	pthread_t threadid;         /* Timer thread ID */
	pthread_mutex_t mutex;      /* Protects timer lists */
  struct ttimer *first;       /* Single linked and sorted list of timers */
};

/* ctx, timer must be valid
 * timer is filled */
static int
_internal_timer_add( struct mg_context *ctx, 
                     struct ttimer *timer)
{
  struct ttimer **ref_last;
  struct ttimer *cur_timer;

  if (ctx->stop_flag) {
    mg_free(timer); /* free timer memory */
    return 0;
  }

  pthread_mutex_lock(&ctx->timers->mutex);

  ref_last = &(ctx->timers->first);
  cur_timer = ctx->timers->first;
  while ( cur_timer) {
    if (timer->time < cur_timer->time) {
      break;
    }
    ref_last = &(cur_timer->next);
    cur_timer = cur_timer->next;
  } /* while */ 
  /* insert timer */
  *ref_last = timer;        
  timer->next = cur_timer;

  pthread_mutex_unlock( &ctx->timers->mutex);
  return 0; /* can not fail */
}

static int
timer_add(struct mg_context *ctx,
          double next_time,
          double period,
          int is_relative,
          taction action,
          void *arg)
{
	struct timespec now;
  double          d;

  if (ctx->stop_flag) {
		return 0;
	}

  /* create timer struct */
  struct ttimer *timer = mg_calloc(sizeof(struct ttimer), 1);
  if (timer == NULL) {
    return -1; /* no memory */
  }

  /* get current time */
  clock_gettime(CLOCK_MONOTONIC, &now);
  d  = now.tv_sec * 1.0;  /* disable compiler warning*/
  d += now.tv_nsec * 1.0E-9;
  /* next_time should never < current time
   * if  is_relative = 0 and next_time = 0 and period = 1 then
   * the timer function will called until next_time >= now without pause
  */
  if (!is_relative && (next_time < d)) {
    next_time = d; /* next_time < current - not allowed */
  }

  if (is_relative) {
		next_time += now.tv_sec;
		next_time += now.tv_nsec * 1.0E-9;
	}

  /* setup timer */
  timer->time   = next_time;
  timer->period = period;
  timer->action = action;
  timer->arg    = arg;
  
  /* insert timer */
  return _internal_timer_add(ctx, timer);
}

static void
timer_thread_run(void *thread_func_param)
{
	struct mg_context *ctx = (struct mg_context *)thread_func_param;
	struct timespec now;
	double d;
	int re_schedule;
	struct ttimer *timer;

	mg_set_thread_name("timer");

	if (ctx->callbacks.init_thread) {
		/* Timer thread */
		ctx->callbacks.init_thread(ctx, 2);
	}

#if defined(HAVE_CLOCK_NANOSLEEP) /* Linux with librt */
	/* TODO */
	while (clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &request, &request)
	       == EINTR) { /*nop*/
		;
	}
#else
	clock_gettime(CLOCK_MONOTONIC, &now);
	d = (double)now.tv_sec + (double)now.tv_nsec * 1.0E-9;
	while (ctx->stop_flag == 0) {

    pthread_mutex_lock( &ctx->timers->mutex);
    /* get first timer => next callback */
    timer = ctx->timers->first;
		if (timer  && d >= timer->time) {
      ctx->timers->first = timer->next;
      /* timer removed from list */
      pthread_mutex_unlock(&ctx->timers->mutex);

			re_schedule = timer->action(timer->arg);
			if (re_schedule && (timer->period > 0)) {
        timer->time += timer->period;
        _internal_timer_add(ctx, timer);
			}
      else {
        mg_free(timer); /* delete timer */
      }
			continue;
		} 
    pthread_mutex_unlock(&ctx->timers->mutex);

    mg_sleep(1);
		clock_gettime(CLOCK_MONOTONIC, &now);
		d = (double)now.tv_sec + (double)now.tv_nsec * 1.0E-9;
	}
#endif
}

#ifdef _WIN32
static unsigned __stdcall timer_thread(void *thread_func_param)
{
	timer_thread_run(thread_func_param);
	return 0;
}
#else
static void *
timer_thread(void *thread_func_param)
{
	timer_thread_run(thread_func_param);
	return NULL;
}
#endif /* _WIN32 */

static int
timers_init(struct mg_context *ctx)
{
	ctx->timers = (struct ttimers *)mg_calloc(sizeof(struct ttimers), 1);
	(void)pthread_mutex_init(&ctx->timers->mutex, NULL);

	/* Start timer thread */
	mg_start_thread_with_id(timer_thread, ctx, &ctx->timers->threadid);

	return 0;
}

static void
timers_exit(struct mg_context *ctx)
{
  struct ttimer *timer;
  if (ctx->timers) {
		(void)pthread_mutex_destroy(&ctx->timers->mutex);
    timer = ctx->timers->first;
    /* free all outstandig timers */
    while (timer) {
      ctx->timers->first = timer->next;
      mg_free(timer);
      timer = ctx->timers->first;
    }
		mg_free(ctx->timers);
	}
}
