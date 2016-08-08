int t1(void *arg)
{
  static int count = 0;
  printf("t1\n");
  count++;
  return  count < 100 ? 1 : 0;
}

int t2(void *arg)
{
  static int count = 0;
  printf("t2=%10d\n", ++count);

  return  1;
}

int t3(void *arg)
{
  static int count = 0;
  printf("t3=%10d\n", ++count);

  return  1;
}

void mg_test_timer(struct mg_context *ctx)
{
  struct timespec now;
  double d;

  clock_gettime(CLOCK_MONOTONIC, &now);
  d = (double)now.tv_sec;
  d += (double)(now.tv_nsec * 1.0E-9);

  timer_add(ctx, d, 1, 0, t1, NULL);
  timer_add(ctx, d, 2, 0, t2, NULL);
  timer_add(ctx, 0, 1, 0, t3, NULL);
}


