#ifndef COUNTER_H
#define COUNTER_H

#include <lvgl.h>

typedef struct {
  uint32_t _last_action_ts;
  uint32_t _initial_action_ts;
  uint32_t * value;
} counter_t;

counter_t counter_create(uint32_t _value);
bool counter_should_change(lv_event_t *e);

#endif
