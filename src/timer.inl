
#if !defined(MAX_TIMERS)
#define MAX_TIMERS MAX_WORKER_THREADS
#endif

typedef void (*taction)(void *arg);

struct timer {
    double time;
    double period;
    taction action;
    void * arg;
};

struct timers {
    pthread_t threadid;               /* Timer thread ID */
    pthread_mutex_t mutex;            /* Protects timer lists */
    struct timer timers[MAX_TIMERS];  /* List of timers */
    unsigned timer_count;             /* Current size of timer list */
};

static void timer_thread_run(void *thread_func_param)
{
    struct mg_context *ctx = (struct mg_context *) thread_func_param;
    struct timespec now;
    double d;
    unsigned u;
    struct timer t;

    while (ctx->stop_flag == 0) {
#if defined(HAVE_CLOCK_NANOSLEEP) /* Linux with librt */
        while (clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &request, &request)==EINTR) {/*nop*/;}
#else
        clock_gettime(CLOCK_MONOTONIC, &now);
        d = (double)now.tv_sec + (double)now.tv_nsec * 1.0E-9;
        for (;;) {
            pthread_mutex_lock(&ctx->timers->mutex);
            if (ctx->timers->timer_count > 0 && d >= ctx->timers->timers[0].time) {
                t = ctx->timers->timers[0];
                for (u=1; u<ctx->timers->timer_count; u++) {
                    ctx->timers->timers[u-1] = ctx->timers->timers[u];
                }
                ctx->timers->timer_count--;
                pthread_mutex_unlock(&ctx->timers->mutex);
                t.action(t.arg);
                continue;
            } else {
                pthread_mutex_unlock(&ctx->timers->mutex);
            }
            mg_sleep(1);
            clock_gettime(CLOCK_MONOTONIC, &now);
            d = (double)now.tv_sec + (double)now.tv_nsec * 1.0E-9;
        }
#endif
    }
}

static int timer_add(struct mg_context * ctx, double time, int is_periodic, int is_relative, taction action, void * arg)
{
    double t = time;
    unsigned u, v;
    int error = 0;
    struct timespec now;

    if (is_relative) {
        clock_gettime(CLOCK_MONOTONIC, &now);
        t += now.tv_sec;
        t += now.tv_nsec * 1.0E-9;
    }

    pthread_mutex_lock(&ctx->timers->mutex);
    if (ctx->timers->timer_count == MAX_TIMERS) {
        error = 1;
    } else {
        for (u=0; u<ctx->timers->timer_count; u++) {
            if (ctx->timers->timers[u].time < time) {
                for (v=ctx->timers->timer_count; v>u; v--) {
                    ctx->timers->timers[v] = ctx->timers->timers[v-1];
                }
                break;
            }
        }
        ctx->timers->timers[u].time = t;
        ctx->timers->timers[u].period = is_periodic ? time : 0.0;
        ctx->timers->timers[u].action = action;
        ctx->timers->timers[u].arg = arg;
        ctx->timers->timer_count++;
    }
    pthread_mutex_unlock(&ctx->timers->mutex);
    return error;
}

#ifdef _WIN32
static unsigned __stdcall timer_thread(void *thread_func_param)
{
    timer_thread_run(thread_func_param);
    return 0;
}
#else
static void *timer_thread(void *thread_func_param)
{
    timer_thread_run(thread_func_param);
    return NULL;
}
#endif /* _WIN32 */

static int timers_init(struct mg_context * ctx)
{
    ctx->timers = (struct timers*) mg_calloc(sizeof(struct timers), 1);
    (void) pthread_mutex_init(&ctx->timers->mutex, NULL);

    /* Start timer thread */
    mg_start_thread_with_id(timer_thread, ctx, &ctx->timers->threadid);

    return 0;
}

static void timers_exit(struct mg_context * ctx)
{
    (void) pthread_mutex_destroy(&ctx->timers->mutex);
    mg_free(ctx->timers);
}
