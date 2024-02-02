#include "duration_mbox.h"
#include "../counter.h"
#include "../helpers.h"
#include "../messages.h"

static counter_t duration;
static lv_obj_t *hours_lb;
static lv_obj_t *minutes_lb;

static void btn_cb(lv_event_t *);
static void increment_cb(lv_event_t *);
static void decrement_cb(lv_event_t *);

void duration_mbox_open(uint32_t initial_value)
{
  duration = counter_create(initial_value);

  static const char *btns[] = {"Apply", "Cancel", ""};
  lv_obj_t *mbox = lv_msgbox_create(NULL, NULL, NULL, btns, false);
  lv_obj_t *mbox_content = lv_msgbox_get_content(mbox);

  lv_obj_add_event_cb(mbox, btn_cb, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_set_style_width(mbox, 200, 0);
  lv_obj_center(mbox);

  lv_obj_set_flex_flow(mbox_content, LV_FLEX_FLOW_ROW);
  lv_obj_center(mbox_content);

  lv_obj_t *decrease_btn = lv_btn_create(mbox_content);
  lv_obj_t *decrease_btn_lb = lv_label_create(decrease_btn);
  lv_label_set_text_fmt(decrease_btn_lb, "-");

  hours_lb = lv_label_create(mbox_content);
  lv_obj_set_size(hours_lb, 45, 40);
  lv_obj_set_style_text_font(hours_lb, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_align(hours_lb, LV_TEXT_ALIGN_RIGHT, 0);

  lv_obj_t *double_dot_lbl = lv_label_create(mbox_content);
  lv_label_set_text_fmt(double_dot_lbl, ":");
  lv_obj_set_size(double_dot_lbl, 10, 40);
  lv_obj_set_style_text_font(double_dot_lbl, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);

  minutes_lb = lv_label_create(mbox_content);
  lv_obj_set_size(minutes_lb, 45, 40);
  lv_obj_set_style_text_font(minutes_lb, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_align(minutes_lb, LV_TEXT_ALIGN_LEFT, 0);

  time_update_lb(*duration.value, hours_lb, minutes_lb);

  lv_obj_t *increase_btn = lv_btn_create(mbox_content);
  lv_obj_t *increase_btn_lb = lv_label_create(increase_btn);
  lv_label_set_text_fmt(increase_btn_lb, "+");

  lv_obj_center(decrease_btn);
  lv_obj_center(hours_lb);
  lv_obj_center(double_dot_lbl);
  lv_obj_center(minutes_lb);
  lv_obj_center(increase_btn);

  // event handlers
  lv_obj_add_event_cb(decrease_btn, increment_cb, LV_EVENT_ALL, &duration);
  lv_obj_add_event_cb(increase_btn, decrement_cb, LV_EVENT_ALL, &duration);
}

void btn_cb(lv_event_t *e)
{
  lv_obj_t * obj = lv_event_get_current_target(e);
  const char * btn_text = lv_msgbox_get_active_btn_text(obj);
  if (strcmp(btn_text, "Apply") == 0) {
    lv_msg_send(MSG_SET_CONF_DURATION_M, duration.value);
  }
  lv_msgbox_close(lv_event_get_current_target(e));
}

void increment_cb(lv_event_t *e)
{
  if (counter_should_change(e))
  {
    counter_t *user_data = (counter_t *)lv_event_get_user_data(e);
    *user_data->value -= (*user_data->value <= DURATION_MIN_VALUE) ? 0 : 1;
    time_update_lb(*user_data->value, hours_lb, minutes_lb);
  }
}

void decrement_cb(lv_event_t *e)
{
  if (counter_should_change(e))
  {
    counter_t *user_data = (counter_t *)lv_event_get_user_data(e);
    *user_data->value += (*user_data->value >= DURATION_MAX_VALUE) ? 0 : 1;
    time_update_lb(*user_data->value, hours_lb, minutes_lb);
  }
}
