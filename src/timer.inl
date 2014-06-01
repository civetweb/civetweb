
struct timers {
    pthread_t timerthreadid;        /* Time thread ID */
    pthread_mutex_t timer_mutex;    /* Protects timer lists */
    struct timer_list *timers;      /* List of timers */
};

static void timer_thread_run(void *thread_func_param)
{
    struct mg_context *ctx = (struct mg_context *) thread_func_param;
    while (ctx->stop_flag == 0) {
        pthread_mutex_lock(&ctx->timer_mutex);
        /* TODO: something useful */
        pthread_mutex_unlock(&ctx->timer_mutex);
        mg_sleep(1);
    }
}

static int timer_add(struct mg_context * ctx, double rel_time, int is_periodic, const char * action)
{
    pthread_mutex_lock(&ctx->timer_mutex);
    /* TODO: something useful */
    pthread_mutex_unlock(&ctx->timer_mutex);
    return 0;
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
    (void) pthread_mutex_init(&ctx->timer_mutex, NULL);

    /* Start timer thread */
    mg_start_thread_with_id(timer_thread, ctx, &ctx->timerthreadid);

    return 0;
}

static void timers_exit(struct mg_context * ctx)
{
    (void) pthread_mutex_destroy(&ctx->timer_mutex);
}
