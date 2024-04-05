
#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>

typedef enum {
  // used for control purposes
  CONTROL_OVEN_START,
  CONTROL_OVEN_PAUSE,
  CONTROL_OVEN_RESUME,
  CONTROL_OVEN_STOP,
  CONTROL_OVEN_SET_BAKING_PROGRAM,
  CONTROL_OVEN_SET_TEMPERATURE,
  CONTROL_OVEN_SET_DURATION_M,
  CONTROL_OVEN_LIGHT_ON,
  CONTROL_OVEN_LIGHT_OFF,
  CONTROL_OVEN_FAN_ON,
  CONTROL_OVEN_FAN_OFF,
  CONTROL_OVEN_TOP_HEATER_ON,
  CONTROL_OVEN_TOP_HEATER_OFF,
  CONTROL_OVEN_DECK_HEATER_ON,
  CONTROL_OVEN_DECK_HEATER_OFF,

  // used for monitor purposes
  MONITOR_UPDATE_OVEN_DATA
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
