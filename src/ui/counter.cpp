#include <stdio.h>

#include "counter.h"

counter_t counter_create(uint32_t _value)
{
  counter_t data;
  data.value = _value;
  return data;
}

static lv_event_code_t events[] = {
    LV_EVENT_PRESSED,
    LV_EVENT_RELEASED,
    LV_EVENT_CLICKED,
    LV_EVENT_SHORT_CLICKED,
    LV_EVENT_PRESSING,
    LV_EVENT_LONG_PRESSED,
    LV_EVENT_LONG_PRESSED_REPEAT};

void counter_add_event_cb(lv_obj_t *o, lv_event_cb_t cb, void *user_data)
{
  short size = sizeof(events) / sizeof(lv_event_code_t);
  for (short i = 0; i < size; i++)
  {
    lv_obj_add_event_cb(o, cb, events[i], user_data);
  }
}

void counter_remove_event_cb(lv_obj_t *o, lv_event_cb_t cb, void *user_data)
{
  lv_obj_remove_event_cb_with_user_data(o, cb, user_data);
}

bool counter_should_change(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  counter_t *user_data = (counter_t *)lv_event_get_user_data(e);

  if (code == LV_EVENT_PRESSED)
  {
    user_data->_initial_action_ts = lv_tick_get(); // retrieve first instant of interaction
    user_data->_last_action_ts = lv_tick_get();
  }
  else if (code == LV_EVENT_RELEASED)
  {
    user_data->_last_action_ts = 0;
    user_data->_initial_action_ts = 0;
  }
  else if (code == LV_EVENT_CLICKED || code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_PRESSING || code == LV_EVENT_LONG_PRESSED || code == LV_EVENT_LONG_PRESSED_REPEAT)
  {
    uint32_t elapsed_since_initial = lv_tick_elaps(user_data->_initial_action_ts);
    uint32_t elapsed_since_last = lv_tick_elaps(user_data->_last_action_ts);

    // do nothing for the first 500ms
    // move slow for the first 3 seconds
    // move fast after 3 seconds
    if (
        elapsed_since_initial > 100 &&
        ((elapsed_since_initial > 5000 && elapsed_since_last > 100) ||
         (elapsed_since_initial > 2000 && elapsed_since_last > 200) ||
         (elapsed_since_initial < 2000 && elapsed_since_initial > 300 && elapsed_since_last > 300) ||
         (elapsed_since_initial < 2000 && elapsed_since_initial < 300 && elapsed_since_last > 200)))
    {
      user_data->_last_action_ts = lv_tick_get();
      return true;
    }
  }
  return false;
}
