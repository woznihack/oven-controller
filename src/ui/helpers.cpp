#include "helpers.h"

void time_update_lb(int minutes, lv_obj_t *hours_lb, lv_obj_t *minutes_lb)
{
  lv_label_set_text_fmt(hours_lb, "%d", minutes / 60);
  if ((minutes % 60) < 10)
  {
    lv_label_set_text_fmt(minutes_lb, "0%d", minutes % 60);
  }
  else
  {
    lv_label_set_text_fmt(minutes_lb, "%d", minutes % 60);
  }
}
