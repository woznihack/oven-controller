#include "temperature_mbox.h"
#include "../counter.h"
#include "../helpers.h"
#include "../messages.h"

static lv_obj_t *temperature_lb;
static counter_t temperature = counter_create(10);

static void temp_msgbox_btn_cb(lv_event_t *);
static void temp_msgbox_dec_cb(lv_event_t *);
static void temp_msgbox_inc_cb(lv_event_t *);

void temperature_mbox_open(lv_event_t *e)
{

  static const char *btns[] = {"Okay", ""};
  lv_obj_t *mbox = lv_msgbox_create(NULL, NULL, NULL, btns, false);
  lv_obj_t *mbox_content = lv_msgbox_get_content(mbox);

  lv_obj_add_event_cb(mbox, temp_msgbox_btn_cb, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_set_style_width(mbox, 200, 0);
  lv_obj_center(mbox);

  lv_obj_set_flex_flow(mbox_content, LV_FLEX_FLOW_ROW);
  lv_obj_center(mbox_content);

  lv_obj_t *decrease_btn = lv_btn_create(mbox_content);
  lv_obj_t *decrease_btn_lb = lv_label_create(decrease_btn);
  lv_label_set_text_fmt(decrease_btn_lb, "-");

  temperature_lb = lv_label_create(mbox_content);
  lv_obj_set_size(temperature_lb, 100, 40);
  lv_obj_set_style_text_font(temperature_lb, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_align(temperature_lb, LV_TEXT_ALIGN_CENTER, 0);
  lv_label_set_text_fmt(temperature_lb, "%d", *temperature.value);

  lv_obj_t *increase_btn = lv_btn_create(mbox_content);
  lv_obj_t *increase_btn_lb = lv_label_create(increase_btn);
  lv_label_set_text_fmt(increase_btn_lb, "+");

  lv_obj_center(decrease_btn);
  lv_obj_center(temperature_lb);
  lv_obj_center(increase_btn);

  // event handlers
  lv_obj_add_event_cb(decrease_btn, temp_msgbox_dec_cb, LV_EVENT_ALL, &temperature);
  lv_obj_add_event_cb(increase_btn, temp_msgbox_inc_cb, LV_EVENT_ALL, &temperature);
}

void temp_msgbox_btn_cb(lv_event_t *e)
{
  lv_msg_send(MSG_TEMPERATURE_T_CHANGED, temperature.value);
  lv_msgbox_close(lv_event_get_current_target(e));
}

void temp_msgbox_dec_cb(lv_event_t *e)
{
  if (counter_should_change(e))
  {
    counter_t *user_data = (counter_t *)lv_event_get_user_data(e);
    *user_data->value -= (*user_data->value <= TEMPERATURE_MIN_VALUE) ? 0 : 1;
    lv_label_set_text_fmt(temperature_lb, "%d", *user_data->value);
  }
}

void temp_msgbox_inc_cb(lv_event_t *e)
{
  if (counter_should_change(e))
  {
    counter_t *user_data = (counter_t *)lv_event_get_user_data(e);
    *user_data->value += (*user_data->value >= TEMPERATURE_MAX_VALUE) ? 0 : 1;
    lv_label_set_text_fmt(temperature_lb, "%d", *user_data->value);
  }
}
