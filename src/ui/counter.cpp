#include <stdio.h>

#include "counter.h"

counter_user_data counter_data_create(int _value){
  counter_user_data data;
  data.value = new int(_value);
  return data;
}

bool counter_event_inspector(lv_event_t *e){
  lv_event_code_t code = lv_event_get_code(e);
  counter_user_data * user_data = (counter_user_data *)lv_event_get_user_data(e);


  if (code == LV_EVENT_PRESSED) {
    user_data->initial_action_ts = lv_tick_get(); // retrieve first instant of interaction
    user_data->last_action_ts = lv_tick_get();
  }
  else if (code == LV_EVENT_RELEASED){
    user_data->last_action_ts = 0;
    user_data->initial_action_ts = 0;
  }
  else if(code == LV_EVENT_CLICKED || code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_PRESSING || code == LV_EVENT_LONG_PRESSED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
    uint32_t elapsed_since_initial = lv_tick_elaps(user_data->initial_action_ts);
    uint32_t elapsed_since_last = lv_tick_elaps(user_data->last_action_ts);

    // do nothing for the first 500ms
    // move slow for the first 3 seconds
    // move fast after 3 seconds
    if (
      elapsed_since_initial > 100 &&
      (
        (elapsed_since_initial > 5000 && elapsed_since_last > 100) ||
        (elapsed_since_initial > 2000 && elapsed_since_last > 200) ||
        (elapsed_since_initial < 2000 && elapsed_since_initial > 300 && elapsed_since_last > 300) ||
        (elapsed_since_initial < 2000 && elapsed_since_initial < 300 && elapsed_since_last > 200)
      )
    ){
      user_data->last_action_ts = lv_tick_get();
      return true;
    }
  }
  return false;
}
