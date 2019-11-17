#include "queue.h"
#include <stdio.h>

/* Private macros. */
// QUEUE* QUEUE_NEXT(QUEUE* q);
// QUEUE* QUEUE_PREV(QUEUE* q);
// QUEUE* QUEUE_PREV_NEXT(QUEUE* q);
// QUEUE* QUEUE_NEXT_PREV(QUEUE* q);
/* Public macros. */
// type* QUEUE_DATA(ptr, type, field);
// QUEUE_FOREACH(QUEUE* node, QUEUE* h);
// bool QUEUE_EMPTY(QUEUE* q);
// QUEUE* QUEUE_HEAD(QUEUE* q);
// void QUEUE_INIT(QUEUE* q);
// void QUEUE_ADD(QUEUE* h, QUEUE* n);
// void QUEUE_SPLIT(QUEUE* h, QUEUE* node, QUEUE* n);
// void QUEUE_MOVE(QUEUE* h, QUEUE* n);
// void QUEUE_INSERT_HEAD(QUEUE* h, QUEUE* q);
// void QUEUE_INSERT_TAIL(QUEUE* h, QUEUE* q);
// void QUEUE_REMOVE(QUEUE* q);

typedef struct User {
  int age;
  char* name;
  QUEUE node;
} User;

int main() {
  QUEUE queue;

  User john;
  john.name = "john";
  john.age = 44;

  User henry;
  henry.name = "henry";
  henry.age = 32;

  QUEUE_INIT(&queue);
  QUEUE_INIT(&john.node);
  QUEUE_INIT(&henry.node);

  ((*(&queue))[0]) = john.node;
  (*(QUEUE**)&((*(&queue))[0])) = &john.node;

  QUEUE_INIT(&queue);
  printf("Is queue empty: %d\n", QUEUE_EMPTY(&queue));
  QUEUE_INSERT_TAIL(&queue, &john.node);
  QUEUE_INSERT_TAIL(&queue, &henry.node);
  printf("Is queue empty: %d\n", QUEUE_EMPTY(&queue));

  QUEUE* q = QUEUE_HEAD(&queue);
  User* user = QUEUE_DATA(q, User, node);
  printf("Received first inserted user: %s who is %d.\n", user->name, user->age);
  QUEUE_REMOVE(q);
  QUEUE_FOREACH(q, &queue) {
    user = QUEUE_DATA(q, User, node);
    printf("Received rest inserted users: %s who is %d.\n", user->name, user->age);
  }

  QUEUE queue2;
  User mary;
  mary.name = "mary";
  mary.age = 24;
  User sana;
  sana.name = "sana";
  sana.age = 22;
  QUEUE_INIT(&queue2);
  QUEUE_INIT(&mary.node);
  QUEUE_INIT(&sana.node);
  QUEUE_INSERT_HEAD(&queue2, &mary.node);
  QUEUE_INSERT_HEAD(&queue2, &sana.node);

  QUEUE_ADD(&queue2, &queue);
  printf("Is queue empty: %d\n", QUEUE_EMPTY(&queue));

  QUEUE_FOREACH(q, &queue2) {
    user = QUEUE_DATA(q, User, node);
    printf("User: %s who is %d.\n", user->name, user->age);
  }

  QUEUE_SPLIT(&queue2, &mary.node, &queue);
  QUEUE qu;
  QUEUE_MOVE(&queue, &qu);
  QUEUE_FOREACH(q, &qu) {
    user = QUEUE_DATA(q, User, node);
    printf("Rest User: %s who is %d.\n", user->name, user->age);
  }

  return 0;
}
