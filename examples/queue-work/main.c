#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <uv.h>

#define FIB_UNTIL 12
uv_loop_t* loop;
pthread_t mainThread;

long fib_(long t) {
  if (t == 0 || t == 1)
    return 1;
  else
    return fib_(t - 1) + fib_(t - 2);
}

void fib(uv_work_t* req) {
  pthread_t fibThread = pthread_self();
  printf("fib: %p, equal main thread: %s\n", fibThread, pthread_equal(fibThread, mainThread) ? "YES" : "NO");
  int n = *(int*)req->data;
  if (random() % 2)
    sleep(1);
  else
    sleep(3);
  long fib = fib_(n);
  fprintf(stderr, "%dth fibonacci is %lu\n", n, fib);
}

void after_fib(uv_work_t* req, int status) {
  (void)status;
  pthread_t afterFibThread = pthread_self();
  printf("after_fib: %p, equal main thread: %s\n",
         afterFibThread,
         pthread_equal(afterFibThread, mainThread) ? "YES" : "NO");
  fprintf(stderr, "Done calculating %dth fibonacci\n", *(int*)req->data);
}

int main() {
  mainThread = pthread_self();
  printf("main thread: %p\n", mainThread);
  loop = uv_default_loop();

  int data[FIB_UNTIL];
  uv_work_t req[FIB_UNTIL];
  int i;
  for (i = 0; i < FIB_UNTIL; i++) {
    data[i] = i;
    req[i].data = (void*)&data[i];
    uv_queue_work(loop, &req[i], fib, after_fib);
  }

  return uv_run(loop, UV_RUN_DEFAULT);
}
