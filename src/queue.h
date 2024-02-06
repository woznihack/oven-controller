
#ifndef QUEUE_H
#define QUEUE_H

// #include <stdio.h>
// #include <stdlib.h>
// #include <stdbool.h>
#include <pthread.h>
// #include <string.h>

typedef enum {
  OVEN_START,
  OVEN_PAUSE,
  OVEN_STOP,
  OVEN_SET_TEMPERATURE,
  OVEN_SET_DURATION_M,
  OVEN_LIGHT_ON,
  OVEN_LIGHT_OFF,
  OVEN_FAN_ON,
  OVEN_FAN_OFF,
  OVEN_BROIL_ON,
  OVEN_BROIL_OFF,

  UI_UPDATE_REMAINING_M,
  UI_UPDATE_TOP_HEATER_TEMP,
  UI_UPDATE_DECK_HEATER_TEMP,
  UI_UPDATE_BAKING_PHASE
} q_event_t;

typedef struct q_node_t {
  void * payload;
  q_event_t event;
  struct q_node_t *next;
} q_node_t;

typedef struct {
  q_node_t * head;
  q_node_t * tail;
  size_t size;
  pthread_mutex_t mutex;
} q_queue_t;

q_queue_t * q_create(size_t size);
bool        q_enqueue(q_queue_t * queue, q_event_t msg, const void * payload);
bool        q_dequeue(q_queue_t * queue, q_node_t * node);

#endif
