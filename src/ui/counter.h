#include <lvgl.h>

typedef struct {
  uint32_t last_action_ts;
  uint32_t initial_action_ts;
  int * value;
} counter_user_data;

counter_user_data counter_data_create(int _value);
bool counter_event_inspector(lv_event_t *e);
