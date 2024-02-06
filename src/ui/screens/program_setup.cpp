#include <stdio.h>
#include <lvgl.h>

#include "screens.h"
#include "../helpers.h"
#include "../messages.h"
#include "../counter.h"


/* PROTOTYPES */
static void increment_duration_cb(lv_event_t *e);
static void decrement_duration_cb(lv_event_t *e);
static void increment_temp_cb(lv_event_t *e);
static void decrement_temp_cb(lv_event_t *e);

/* GLOBALS */
lv_obj_t *program_setup_scr;

/* STATIC GLOBAL VARIABLES */
static lv_obj_t *temp_container;
static lv_obj_t *temp_lb;
static lv_obj_t *temp_inc_btn;
static lv_obj_t *temp_dec_btn;

static lv_obj_t *duration_container;
static lv_obj_t *duration_inc_btn;
static lv_obj_t *duration_dec_btn;
static lv_obj_t *duration_h_lb;
static lv_obj_t *duration_m_lb;
static lv_obj_t *duration_separator_lb;

static lv_obj_t *start_btn;

void send_start_msg_cb(lv_event_t *e){
  LV_UNUSED(e);
  lv_msg_send(MSG_SET_STATUS_STARTED, NULL);
}

static void on_load(lv_event_t *e){
  // add callbacks
  counter_add_event_cb(temp_dec_btn, decrement_temp_cb, &settings.temperature);
  counter_add_event_cb(temp_inc_btn, increment_temp_cb, &settings.temperature);
  counter_add_event_cb(duration_dec_btn, decrement_duration_cb, &settings.duration_m);
  counter_add_event_cb(duration_inc_btn, increment_duration_cb, &settings.duration_m);
  lv_obj_add_event_cb(start_btn, send_start_msg_cb, LV_EVENT_CLICKED, NULL);

  // refresh data
  lv_label_set_text_fmt(temp_lb, "%d °C", (int)settings.temperature.value);
  time_update_lb(settings.duration_m.value, duration_h_lb, duration_m_lb);
}

static void on_unload(lv_event_t *e){
  // remove callbacks
  counter_remove_event_cb(temp_dec_btn, decrement_temp_cb, &settings.temperature);
  counter_remove_event_cb(temp_inc_btn, increment_temp_cb, &settings.temperature);
  counter_remove_event_cb(duration_dec_btn, decrement_duration_cb, &settings.duration_m);
  counter_remove_event_cb(duration_inc_btn, increment_duration_cb, &settings.duration_m);
  lv_obj_remove_event_cb(start_btn, send_start_msg_cb);
}

void program_setup_scr_init(){
  printf("-- INITIALIZING PROGRAM SETUP ---");
  printf("Setting.temperature = %d\n", (int)settings.temperature.value);
  printf("Setting.duration_m = %d\n", settings.duration_m.value);

  program_setup_scr = lv_obj_create(NULL);
  lv_obj_set_scrollbar_mode(program_setup_scr, LV_SCROLLBAR_MODE_OFF);
  lv_obj_clear_flag(program_setup_scr, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_add_event_cb(program_setup_scr, on_load, LV_EVENT_SCREEN_LOADED, NULL);
  lv_obj_add_event_cb(program_setup_scr, on_unload, LV_EVENT_SCREEN_UNLOADED, NULL);

  temp_container = lv_obj_create(program_setup_scr);
  lv_obj_set_size(temp_container, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_pos(temp_container, lv_pct(5), lv_pct(20));

  temp_dec_btn = lv_btn_create(temp_container);
  lv_obj_t *temp_dec_btn_lb = lv_label_create(temp_dec_btn);
  lv_label_set_text_fmt(temp_dec_btn_lb, "-");

  temp_inc_btn = lv_btn_create(temp_container);
  lv_obj_t *temp_inc_btn_lb = lv_label_create(temp_inc_btn);
  lv_label_set_text_fmt(temp_inc_btn_lb, "+");

  temp_lb = lv_label_create(temp_container);


  lv_obj_align(temp_lb, LV_ALIGN_LEFT_MID, 0, 0);
  lv_obj_align_to(temp_dec_btn, temp_lb, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
  lv_obj_align_to(temp_inc_btn, temp_dec_btn, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

  duration_container = lv_obj_create(program_setup_scr);
  lv_obj_set_size(duration_container, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_pos(duration_container, lv_pct(5), lv_pct(40));

  duration_h_lb = lv_label_create(duration_container);
  lv_obj_set_style_text_align(duration_h_lb, LV_TEXT_ALIGN_RIGHT, LV_STATE_DEFAULT);
  duration_m_lb = lv_label_create(duration_container);
  duration_separator_lb = lv_label_create(duration_container);

  time_update_lb(settings.duration_m.value, duration_h_lb, duration_m_lb);
  lv_label_set_text_fmt(duration_separator_lb, ":");

  duration_dec_btn = lv_btn_create(duration_container);
  lv_obj_t *duration_dec_btn_lb = lv_label_create(duration_dec_btn);
  lv_label_set_text_fmt(duration_dec_btn_lb, "-");

  duration_inc_btn = lv_btn_create(duration_container);
  lv_obj_t *duration_inc_btn_lb = lv_label_create(duration_inc_btn);
  lv_label_set_text_fmt(duration_inc_btn_lb, "+");

  lv_obj_align(duration_h_lb, LV_ALIGN_LEFT_MID, 0, 0);
  lv_obj_align_to(duration_separator_lb, duration_h_lb, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
  lv_obj_align_to(duration_m_lb, duration_separator_lb, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
  lv_obj_align_to(duration_dec_btn, duration_m_lb, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
  lv_obj_align_to(duration_inc_btn, duration_dec_btn, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

  lv_obj_t *btns_container = lv_obj_create(program_setup_scr);
  lv_obj_set_size(btns_container, lv_pct(100), LV_SIZE_CONTENT);
  lv_obj_set_pos(btns_container, 0, lv_pct(100) - 30);

  start_btn = lv_btn_create(btns_container);
  lv_obj_t *start_btn_lb = lv_label_create(start_btn);
  lv_label_set_text_fmt(start_btn_lb, "Start");
  lv_obj_center(start_btn);
}

static void decrement_duration_cb(lv_event_t *e)
{
  if (counter_should_change(e))
  {
    counter_t *user_data = (counter_t *)lv_event_get_user_data(e);
    user_data->value -= (user_data->value <= BAKING_DURATION_M_MIN) ? 0 : 1;
    time_update_lb(user_data->value, duration_h_lb, duration_m_lb);
  }
}

static void increment_duration_cb(lv_event_t *e)
{
  if (counter_should_change(e))
  {
    counter_t *user_data = (counter_t *)lv_event_get_user_data(e);
    user_data->value += (user_data->value >= BAKING_DURATION_M_MAX) ? 0 : 1;
    time_update_lb(user_data->value, duration_h_lb, duration_m_lb);
  }
}

static void decrement_temp_cb(lv_event_t *e)
{
  if (counter_should_change(e))
  {
    counter_t *user_data = (counter_t *)lv_event_get_user_data(e);
    user_data->value -= (user_data->value <= BAKING_TEMPERATURE_MIN) ? 0 : 1;
    lv_label_set_text_fmt(temp_lb, "%d °C", user_data->value);
  }
}

static void increment_temp_cb(lv_event_t *e)
{
  if (counter_should_change(e))
  {
    counter_t *user_data = (counter_t *)lv_event_get_user_data(e);
    user_data->value += (user_data->value >= BAKING_TEMPERATURE_MAX) ? 0 : 1;
    lv_label_set_text_fmt(temp_lb, "%d °C", user_data->value);
  }
}
