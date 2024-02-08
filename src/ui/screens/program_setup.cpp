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
static lv_obj_t *title_lb;
static lv_obj_t *temp_container;
static lv_obj_t *temp_lb;
static lv_obj_t *temp_inc_btn;
static lv_obj_t *temp_dec_btn;

static lv_obj_t *duration_container;
static lv_obj_t *duration_inc_btn;
static lv_obj_t *duration_dec_btn;
static lv_obj_t *duration_lb;

static lv_obj_t *start_btn;

void send_start_msg_cb(lv_event_t *e)
{
  LV_UNUSED(e);
  lv_msg_send(MSG_SET_STATUS_STARTED, NULL);
}

static void on_load(lv_event_t *e)
{
  // add callbacks
  counter_add_event_cb(temp_dec_btn, decrement_temp_cb, &settings.temperature);
  counter_add_event_cb(temp_inc_btn, increment_temp_cb, &settings.temperature);
  counter_add_event_cb(duration_dec_btn, decrement_duration_cb, &settings.duration_m);
  counter_add_event_cb(duration_inc_btn, increment_duration_cb, &settings.duration_m);
  lv_obj_add_event_cb(start_btn, send_start_msg_cb, LV_EVENT_CLICKED, NULL);
}

static void on_unload(lv_event_t *e)
{
  // remove callbacks
  counter_remove_event_cb(temp_dec_btn, decrement_temp_cb, &settings.temperature);
  counter_remove_event_cb(temp_inc_btn, increment_temp_cb, &settings.temperature);
  counter_remove_event_cb(duration_dec_btn, decrement_duration_cb, &settings.duration_m);
  counter_remove_event_cb(duration_inc_btn, increment_duration_cb, &settings.duration_m);
  lv_obj_remove_event_cb(start_btn, send_start_msg_cb);
}

void program_setup_scr_init()
{

  program_setup_scr = lv_obj_create(NULL);

  lv_obj_add_event_cb(program_setup_scr, on_load, LV_EVENT_SCREEN_LOADED, NULL);
  lv_obj_add_event_cb(program_setup_scr, on_unload, LV_EVENT_SCREEN_UNLOADED, NULL);

  lv_obj_t *main_container = lv_obj_create(program_setup_scr);
  lv_obj_set_size(main_container, 240, 320);
  lv_obj_center(main_container);
  lv_obj_set_flex_flow(main_container, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_scrollbar_mode(main_container, LV_SCROLLBAR_MODE_OFF);
  lv_obj_clear_flag(main_container, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_pad_all(main_container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(main_container, lv_color_hex(0x484848), LV_PART_MAIN);
  lv_obj_set_style_pad_row(main_container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  static lv_style_t container_style;
  lv_style_init(&container_style);
  lv_style_set_radius(&container_style, 0);
  lv_style_set_bg_opa(&container_style, LV_OPA_0);
  lv_style_set_outline_width(&container_style, 0);
  lv_style_set_border_width(&container_style, 0);
  lv_style_set_border_opa(&container_style, LV_OPA_30);
  lv_style_set_pad_all(&container_style, 0);
  lv_style_set_text_color(&container_style, lv_color_white());
  lv_style_set_align(&container_style, LV_ALIGN_CENTER);
  lv_style_set_pad_column(&container_style, 2);
  lv_style_set_text_align(&container_style, LV_TEXT_ALIGN_CENTER);

  static lv_style_t btns_style;
  lv_style_init(&btns_style);
  lv_style_set_radius(&btns_style, 5);
  lv_style_set_width(&btns_style, 30);
  lv_style_set_height(&btns_style, 30);
  lv_style_set_bg_opa(&btns_style, LV_OPA_0);

  static lv_style_t btns_medium_style;
  lv_style_init(&btns_medium_style);
  lv_style_set_radius(&btns_medium_style, 5);
  lv_style_set_width(&btns_medium_style, 40);
  lv_style_set_height(&btns_medium_style, 40);

  static lv_style_t btns_circle_style;
  lv_style_init(&btns_circle_style);
  lv_style_set_radius(&btns_circle_style, 40);
  lv_style_set_width(&btns_circle_style, 40);
  lv_style_set_height(&btns_circle_style, 40);

  lv_obj_t *title_container = lv_obj_create(main_container);
  lv_obj_set_size(title_container, LV_PCT(100), 30);
  lv_obj_add_style(title_container, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_t *step_setup_container = lv_obj_create(main_container);
  lv_obj_set_flex_flow(step_setup_container, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(step_setup_container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_pad_column(step_setup_container, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_size(step_setup_container, LV_PCT(100), 40);
  lv_obj_add_style(step_setup_container, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_t *temperature_setup_container = lv_obj_create(main_container);
  lv_obj_set_flex_flow(temperature_setup_container, LV_FLEX_FLOW_ROW);
  lv_obj_set_width(temperature_setup_container, LV_PCT(100));
  lv_obj_set_flex_grow(temperature_setup_container, 1);
  lv_obj_add_style(temperature_setup_container, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_t *duration_setup_container = lv_obj_create(main_container);
  lv_obj_set_flex_flow(duration_setup_container, LV_FLEX_FLOW_ROW);
  lv_obj_set_width(duration_setup_container, LV_PCT(100));
  lv_obj_set_flex_grow(duration_setup_container, 1);
  lv_obj_add_style(duration_setup_container, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_t *feature_setup_container = lv_obj_create(main_container);
  lv_obj_set_flex_flow(feature_setup_container, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(feature_setup_container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_size(feature_setup_container, LV_PCT(100), 50);
  lv_obj_add_style(feature_setup_container, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_t *controls_container = lv_obj_create(main_container);
  lv_obj_set_flex_flow(controls_container, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(controls_container, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_size(controls_container, LV_PCT(100), 50);
  lv_obj_add_style(controls_container, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  // TITLE CONTAINER
  title_lb = lv_label_create(title_container);
  lv_label_set_text_fmt(title_lb, "BAKE CONFIGURATION");
  lv_obj_center(title_lb);

  // STEPS SETUP CONTAINER
  lv_obj_t *prev_step_btn = lv_btn_create(step_setup_container);
  lv_obj_t *prev_step_btn_lb = lv_label_create(prev_step_btn);
  lv_label_set_text_fmt(prev_step_btn_lb, "<");
  lv_obj_add_style(prev_step_btn, &btns_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_t *step_name_lb = lv_label_create(step_setup_container);
  lv_label_set_text_fmt(step_name_lb, "step 1/1");
  lv_obj_set_flex_grow(step_name_lb, 1);

  lv_obj_t *next_step_btn = lv_btn_create(step_setup_container);
  lv_obj_t *next_step_btn_lb = lv_label_create(next_step_btn);
  lv_label_set_text_fmt(next_step_btn_lb, ">");
  lv_obj_add_style(next_step_btn, &btns_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_t *add_step_btn = lv_btn_create(step_setup_container);
  lv_obj_t *add_step_btn_lb = lv_label_create(add_step_btn);
  lv_label_set_text_fmt(add_step_btn_lb, "ADD STEP");
  lv_obj_add_style(add_step_btn, &btns_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_t *del_step_btn = lv_btn_create(step_setup_container);
  lv_obj_t *del_step_btn_lb = lv_label_create(del_step_btn);
  lv_label_set_text_fmt(del_step_btn_lb, "DEL STEP");
  lv_obj_add_style(del_step_btn, &btns_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  // TEMPERATURE SETUP CONTAINER
  lv_obj_t *temp_dec_btn_container = lv_obj_create(temperature_setup_container);
  lv_obj_set_height(temp_dec_btn_container, LV_PCT(100));
  lv_obj_set_flex_grow(temp_dec_btn_container, 1);
  lv_obj_add_style(temp_dec_btn_container, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  temp_dec_btn = lv_btn_create(temp_dec_btn_container);
  lv_obj_set_align(temp_dec_btn, LV_ALIGN_CENTER);
  lv_obj_add_style(temp_dec_btn, &btns_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(temp_dec_btn, &btns_circle_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_t *temp_dec_btn_lb = lv_label_create(temp_dec_btn);
  lv_label_set_text_fmt(temp_dec_btn_lb, "-");

  lv_obj_t *temp_lb_container = lv_obj_create(temperature_setup_container);
  lv_obj_set_height(temp_lb_container, LV_PCT(100));
  lv_obj_set_flex_grow(temp_lb_container, 3);
  lv_obj_add_style(temp_lb_container, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  temp_lb = lv_label_create(temp_lb_container);
  lv_obj_set_style_text_font(temp_lb, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_align(temp_lb, LV_ALIGN_CENTER);
  lv_label_set_text_fmt(temp_lb, "%d °C", (int)settings.temperature.value);

  lv_obj_t *temp_inc_btn_container = lv_obj_create(temperature_setup_container);
  lv_obj_set_height(temp_inc_btn_container, LV_PCT(100));
  lv_obj_set_flex_grow(temp_inc_btn_container, 1);
  lv_obj_add_style(temp_inc_btn_container, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  temp_inc_btn = lv_btn_create(temp_inc_btn_container);
  lv_obj_add_style(temp_inc_btn, &btns_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(temp_inc_btn, &btns_circle_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_align(temp_inc_btn, LV_ALIGN_CENTER);
  lv_obj_t *temp_inc_btn_lb = lv_label_create(temp_inc_btn);
  lv_label_set_text_fmt(temp_inc_btn_lb, "+");


  // DURATION SETUP CONTAINER
  lv_obj_t *duration_dec_btn_container = lv_obj_create(duration_setup_container);
  lv_obj_set_height(duration_dec_btn_container, LV_PCT(100));
  lv_obj_set_flex_grow(duration_dec_btn_container, 1);
  lv_obj_add_style(duration_dec_btn_container, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  duration_dec_btn = lv_btn_create(duration_dec_btn_container);
  lv_obj_set_align(duration_dec_btn, LV_ALIGN_CENTER);
  lv_obj_add_style(duration_dec_btn, &btns_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_t *duration_dec_btn_lb = lv_label_create(duration_dec_btn);
  lv_label_set_text_fmt(duration_dec_btn_lb, "-");

  lv_obj_t *duration_lb_container = lv_obj_create(duration_setup_container);
  lv_obj_set_height(duration_lb_container, LV_PCT(100));
  lv_obj_set_flex_grow(duration_lb_container, 3);
  lv_obj_add_style(duration_lb_container, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  duration_lb = lv_label_create(duration_lb_container);
  lv_obj_set_style_text_font(duration_lb, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_align(duration_lb, LV_ALIGN_CENTER);
  lv_label_set_text_fmt(duration_lb, "%d:%02d", (int)settings.duration_m.value/60, (int)settings.duration_m.value%60);

  lv_obj_t *duration_inc_btn_container = lv_obj_create(duration_setup_container);
  lv_obj_set_height(duration_inc_btn_container, LV_PCT(100));
  lv_obj_set_flex_grow(duration_inc_btn_container, 1);
  lv_obj_add_style(duration_inc_btn_container, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  duration_inc_btn = lv_btn_create(duration_inc_btn_container);
  lv_obj_add_style(duration_inc_btn, &btns_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_align(duration_inc_btn, LV_ALIGN_CENTER);
  lv_obj_t *duration_inc_btn_lb = lv_label_create(duration_inc_btn);
  lv_label_set_text_fmt(duration_inc_btn_lb, "+");

  // FEATURES SETUP CONTAINER
  lv_obj_t *light_btn = lv_btn_create(feature_setup_container);
  lv_obj_add_style(light_btn, &btns_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(light_btn, &btns_medium_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_t *light_btn_lb = lv_label_create(light_btn);
  lv_label_set_text_fmt(light_btn_lb, "L");

  lv_obj_t *fan_btn = lv_btn_create(feature_setup_container);
  lv_obj_add_style(fan_btn, &btns_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(fan_btn, &btns_medium_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_t *fan_btn_lb = lv_label_create(fan_btn);
  lv_label_set_text_fmt(fan_btn_lb, "F");

  lv_obj_t *grill_btn = lv_btn_create(feature_setup_container);
  lv_obj_add_style(grill_btn, &btns_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(grill_btn, &btns_medium_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_t *grill_btn_lb = lv_label_create(grill_btn);
  lv_label_set_text_fmt(grill_btn_lb, "G");

  lv_obj_t *top_heater_btn = lv_btn_create(feature_setup_container);
  lv_obj_add_style(top_heater_btn, &btns_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(top_heater_btn, &btns_medium_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_t *top_heater_btn_lb = lv_label_create(top_heater_btn);
  lv_label_set_text_fmt(top_heater_btn_lb, "TH");

  lv_obj_t *deck_heater_btn = lv_btn_create(feature_setup_container);
  lv_obj_add_style(deck_heater_btn, &btns_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(deck_heater_btn, &btns_medium_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_t *deck_heater_btn_lb = lv_label_create(deck_heater_btn);
  lv_label_set_text_fmt(deck_heater_btn_lb, "DH");


  // CONTROLS
  start_btn = lv_btn_create(controls_container);
  lv_obj_t *start_btn_lb = lv_label_create(start_btn);
  lv_label_set_text_fmt(start_btn_lb, "Start");
}

static void decrement_duration_cb(lv_event_t *e)
{
  if (counter_should_change(e))
  {
    counter_t *user_data = (counter_t *)lv_event_get_user_data(e);
    user_data->value -= (user_data->value <= BAKING_DURATION_M_MIN) ? 0 : 1;
    lv_label_set_text_fmt(duration_lb, "%0d:%02d", (int)user_data->value/60, (int)user_data->value%60);

  }
}

static void increment_duration_cb(lv_event_t *e)
{
  if (counter_should_change(e))
  {
    counter_t *user_data = (counter_t *)lv_event_get_user_data(e);
    user_data->value += (user_data->value >= BAKING_DURATION_M_MAX) ? 0 : 1;
    lv_label_set_text_fmt(duration_lb, "%d:%02d", (int)user_data->value/60, (int)user_data->value%60);
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
