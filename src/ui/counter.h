#ifndef COUNTER_H
#define COUNTER_H

#include <lvgl.h>

typedef struct
{
  uint32_t _last_action_ts;
  uint32_t _initial_action_ts;
  uint32_t value;
} counter_t;

counter_t counter_create(uint32_t _value);
bool counter_should_change(lv_event_t *e);
void counter_add_event_cb(lv_obj_t *o, lv_event_cb_t cb, void *user_data);
void counter_remove_event_cb(lv_obj_t *o, lv_event_cb_t cb, void *user_data);

#endif
