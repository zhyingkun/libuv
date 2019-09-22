#include <stdio.h>
#include <uv.h>

int64_t counter = 0;

void wait_for_a_while(uv_idle_t* handle) {
  if (counter == 0) {
    printf("In wait_for_a_while\n");
  }
  counter++;

  if (counter >= 10e2)
    uv_idle_stop(handle);
}

static void scan_all_uv_handle(uv_handle_t* handle, void* arg) {
  printf("======================================\n");
  printf("handle: %p arg: %p *arg: %d\n", handle, arg, *(int*)arg);
  printf("handle->type: %d handle->u.fd: %d\n", handle->type, handle->u.fd);
  printf("handle->loop: %p handle->flags: %ud\n", handle->loop, handle->flags);
}

int main() {
  printf("Default loop: %p\n", uv_default_loop());
  uv_idle_t idler;

  uv_idle_init(uv_default_loop(), &idler);
  uv_idle_start(&idler, wait_for_a_while);

  printf("Idling...\n");
  uv_run(uv_default_loop(), UV_RUN_DEFAULT);

  int arg = 42;
  uv_walk(uv_default_loop(), scan_all_uv_handle, &arg);

  counter = 0;
  uv_idle_start(&idler, wait_for_a_while);
  uv_run(uv_default_loop(), UV_RUN_DEFAULT);

  uv_loop_close(uv_default_loop());
  return 0;
}
