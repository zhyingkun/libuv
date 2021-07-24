#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <uv.h>

int alloc_count = 0;
static void* stick_malloc(size_t sz) {
  alloc_count++;
  return malloc(sz);
}
static void stick_free(void* ptr) {
  alloc_count--;
  free(ptr);
}

void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
  (void)handle;
  buf->base = (char*)stick_malloc(suggested_size);
  buf->len = suggested_size;
}
void on_close(uv_handle_t* handle) {
  stick_free(handle);
}

void tcp_on_read(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf) {
  if (nread > 0) {
    buf->base[nread] = '\0';
    printf("tcp_on_read: %ld, %s\nEND\n", nread, buf->base);
  } else if (nread < 0) {
    if (nread != UV_EOF)
      fprintf(stderr, "Read error %s\n", uv_err_name(nread));
  }
  stick_free(buf->base);
  uv_close((uv_handle_t*)client, on_close);
  uv_stop(client->loop);
}
void on_new_connection(uv_stream_t* server, int status) {
  if (status < 0) {
    fprintf(stderr, "New connection error %s\n", uv_strerror(status));
    uv_close((uv_handle_t*)server, on_close);
    return;
  }

  uv_loop_t* loop = uv_default_loop();
  uv_tcp_t* client = (uv_tcp_t*)stick_malloc(sizeof(uv_tcp_t));
  uv_tcp_init(loop, client);
  if (uv_accept(server, (uv_stream_t*)client) == 0) {
    uv_read_start((uv_stream_t*)client, alloc_buffer, tcp_on_read);
  } else {
    uv_close((uv_handle_t*)client, on_close);
  }
  uv_close((uv_handle_t*)server, on_close);
}

static int udp_count = 0;
static void udp_on_read(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* addr,
                        unsigned flags) {
  (void)addr;
  (void)flags;
  udp_count++;
  if (nread < 0) {
    fprintf(stderr, "Read error %s\n", uv_err_name(nread));
  } else {
    buf->base[nread] = '\0';
    printf("udp_on_read: %ld, %s\nEND: %p \n\n", nread, buf->base, addr);
  }
  stick_free(buf->base);
  if (udp_count >= 6) {
    uv_close((uv_handle_t*)handle, on_close);
  } else {
    uv_udp_recv_start(handle, alloc_buffer, udp_on_read);
  }
}

static void start_server() {
  printf("%s\n", __FUNCTION__);
  fflush(stdout);

  uv_loop_t* loop = uv_default_loop();
  uv_tcp_t* server = (uv_tcp_t*)stick_malloc(sizeof(uv_tcp_t));

  uv_tcp_init(loop, server);
  struct sockaddr_in addr[1];
  uv_ip4_addr("0.0.0.0", 1024, addr);
  uv_tcp_bind(server, (const struct sockaddr*)addr, 0);

  int r = uv_listen((uv_stream_t*)server, 128, on_new_connection);
  if (r) {
    fprintf(stderr, "TCP Listen error %s\n", uv_strerror(r));
  }

  uv_udp_t* recv_socket = (uv_udp_t*)stick_malloc(sizeof(uv_udp_t));

  uv_udp_init(loop, recv_socket);
  struct sockaddr_in recv_addr[1];
  uv_ip4_addr("0.0.0.0", 1025, recv_addr);
  uv_udp_bind(recv_socket, (const struct sockaddr*)recv_addr, UV_UDP_REUSEADDR);
  uv_udp_recv_start(recv_socket, alloc_buffer, udp_on_read);
}

char* data = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrlsuvwxyz====";
static uv_buf_t buf[1];

static int tcp_count = 0;
static void tcp_on_write(uv_write_t* req, int status) {
  printf("tcp_on_write: %d\n", status);
  tcp_count++;
  if (tcp_count >= 2) {
    uv_close((uv_handle_t*)req->handle, on_close);
  }
  stick_free(req);
}

static void tcp_connect_cb(uv_connect_t* req, int status) {
  (void)req;
  printf("tcp_connect_cb: %s\n", uv_err_name(status));
  if (status == 0) {
    uv_write_t* write_req = (uv_write_t*)stick_malloc(sizeof(uv_write_t));
    uv_write_t* write_req1 = (uv_write_t*)stick_malloc(sizeof(uv_write_t));
    buf[0].base = data;
    buf[0].len = strlen(data);
    uv_write(write_req, req->handle, buf, 1, tcp_on_write);
    uv_write(write_req1, req->handle, buf, 1, tcp_on_write);
  }
  stick_free(req);
}

static void start_client_tcp() {
  printf("%s\n", __FUNCTION__);
  fflush(stdout);

  uv_loop_t* loop = uv_default_loop();
  uv_tcp_t* client = (uv_tcp_t*)stick_malloc(sizeof(uv_tcp_t));
  uv_tcp_init(loop, client);
  struct sockaddr_in addr[1];
  uv_ip4_addr("0.0.0.0", 1024, addr);
  uv_connect_t* tcp_connect = (uv_connect_t*)stick_malloc(sizeof(uv_connect_t));

  uv_tcp_connect(tcp_connect, client, (const struct sockaddr*)addr, tcp_connect_cb);
}

static int udp_send_count = 0;
static void udp_on_send(uv_udp_send_t* req, int status) {
  //    int optlen = sizeof(int);
  //    int optval;
  //    getsockopt(send_socket.u.fd, SOL_SOCKET, SO_SNDBUF, &optval, &optlen);
  printf("udp_on_send: %d, %s\n", status, uv_err_name(status));
  udp_send_count++;
  if (udp_send_count >= 3) {
    uv_close((uv_handle_t*)req->handle, on_close);
  }
  stick_free(req);
}

static uv_buf_t udp_buf[2];
static uv_buf_t udp_buf1[1];

static void udp_on_send1(uv_udp_send_t* req, int status) {
  udp_on_send(req, status);
  stick_free(udp_buf1[0].base);
}

static void start_client_udp() {
  printf("%s\n", __FUNCTION__);
  fflush(stdout);

  uv_loop_t* loop = uv_default_loop();
  uv_udp_t* send_socket = (uv_udp_t*)stick_malloc(sizeof(uv_udp_t));

  uv_udp_init(loop, send_socket);
  struct sockaddr_in broadcast_addr[1];
  uv_ip4_addr("0.0.0.0", 0, broadcast_addr);
  uv_udp_bind(send_socket, (const struct sockaddr*)broadcast_addr, 0);
  uv_udp_set_broadcast(send_socket, 1);

  size_t len = strlen(data);
  udp_buf[0].base = data;
  udp_buf[0].len = len;
  udp_buf[1].base = data;
  udp_buf[1].len = len;

  uv_udp_send_t* send_req = (uv_udp_send_t*)stick_malloc(sizeof(uv_udp_send_t));
  uv_udp_send_t* send_req1 = (uv_udp_send_t*)stick_malloc(sizeof(uv_udp_send_t));
  uv_udp_send_t* send_req2 = (uv_udp_send_t*)stick_malloc(sizeof(uv_udp_send_t));

  struct sockaddr_in send_addr[1];
  uv_ip4_addr("127.0.0.1", 1025, send_addr);
  //    uv_ip4_addr("10.45.55.39", 1025, &send_addr);
  //    uv_ip4_addr("255.255.255.255", 1025, &send_addr);
  uv_udp_send(send_req, send_socket, udp_buf, 1, (const struct sockaddr*)send_addr, udp_on_send);
  uv_udp_send(send_req1, send_socket, udp_buf, 2, (const struct sockaddr*)send_addr, udp_on_send);
  char* long_data = (char*)stick_malloc(66 * 1024 + 1);
  int i;
  for (i = 0; i < 1024; i++) {
    memcpy(long_data + i * 66, data, 66);
  }
  size_t msg_size = 9; // error on 9217
  long_data[msg_size] = '\0';
  udp_buf1[0].base = long_data;
  udp_buf1[0].len = msg_size;
  uv_udp_send(send_req2, send_socket, udp_buf1, 1, (const struct sockaddr*)&send_addr, udp_on_send1);
}

int main(int argc, const char* argv[]) {
  if (argc == 1) {
    start_server();
  } else if (argc == 2) {
    if (strcmp(argv[1], "-t") == 0) {
      start_client_tcp();
    } else if (strcmp(argv[1], "-u") == 0) {
      start_client_udp();
    }
  }
  int result = uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  printf("memory alloc count: %d\n", alloc_count);
  return result;
}
