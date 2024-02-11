#include <stdio.h>
#include <lvgl.h>

#include "screens.h"
#include "../helpers.h"
#include "../messages.h"
#include "../counter.h"

/* GLOBALS */
lv_obj_t *baking_scr;

/* utils */
static void btn_set_hidden_soft(lv_obj_t *obj);
static void btn_clear_hidden_soft(lv_obj_t *obj);
static void btn_set_hidden(lv_obj_t *obj);
static void btn_clear_hidden(lv_obj_t *obj);
static void scr_load_cb(lv_event_t *e);
static void scr_unload_cb(lv_event_t *e);

/* styles */
static lv_style_t container_style;
static lv_style_t btns_style;
static lv_style_t btns_checked_style;
static lv_style_t btns_medium_style;
static lv_style_t btns_circle_style;

/* screen layout */
static lv_obj_t *scr_content;
static lv_obj_t *top_bar;
static lv_obj_t *steps_bar;
static lv_obj_t *temperature_picker;
static lv_obj_t *duration_picker;
static lv_obj_t *toggles_bar;
static lv_obj_t *bottom_bar;

/* event callbacks */
static void refresh_ui();

/* main */
static lv_obj_t *title_lb;
static void init_styles();
static void draw_screen_layout();
static void draw_screen_items();

/* steps bar */
static lv_obj_t *step_name_lb;

/* temperature monitor */
static lv_obj_t *curr_temp_lb;
static lv_obj_t *temp_lb;
static lv_obj_t *temp_inc_btn;
static lv_obj_t *temp_dec_btn;
static void temp_inc_cb(lv_event_t *e);
static void temp_dec_cb(lv_event_t *e);

/* duration monitor */
static lv_obj_t *duration_inc_btn;
static lv_obj_t *duration_dec_btn;
static lv_obj_t *duration_lb;
static void duration_inc_cb(lv_event_t *e);
static void duration_dec_cb(lv_event_t *e);

/* toggles bar */
static lv_obj_t *toggle_light_btn;
static lv_obj_t *toggle_fan_btn;
static lv_obj_t *toggle_grill_btn;
static lv_obj_t *toggle_top_heater_btn;
static lv_obj_t *toggle_deck_heater_btn;
static void toggle_light_cb(lv_event_t *e);
static void toggle_fan_cb(lv_event_t *e);
static void toggle_grill_cb(lv_event_t *e);
static void toggle_top_heater_cb(lv_event_t *e);
static void toggle_deck_heater_cb(lv_event_t *e);

/* bottom bar */
static lv_obj_t *pause_btn;
static lv_obj_t *resume_btn;
static lv_obj_t *stop_btn;
static void stop_cb(lv_event_t *e);
static void pause_cb(lv_event_t *e);
static void resume_cb(lv_event_t *e);

void oven_data_ready_cb(void *, lv_msg_t *){
  refresh_ui();
}

void baking_scr_init()
{
  baking_scr = lv_obj_create(NULL);
  init_styles();
  draw_screen_layout();
  draw_screen_items();

  lv_obj_add_event_cb(baking_scr, scr_load_cb, LV_EVENT_SCREEN_LOADED, NULL);
  lv_obj_add_event_cb(baking_scr, scr_unload_cb, LV_EVENT_SCREEN_UNLOADED, NULL);

  refresh_ui();
}

static void scr_load_cb(lv_event_t *e)
{
  // add callbacks

  lv_obj_add_event_cb(temp_inc_btn, temp_inc_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(temp_dec_btn, temp_dec_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(duration_inc_btn, duration_inc_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(duration_dec_btn, duration_dec_cb, LV_EVENT_CLICKED, NULL);

  lv_obj_add_event_cb(toggle_light_btn, toggle_light_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(toggle_fan_btn, toggle_fan_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(toggle_grill_btn, toggle_grill_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(toggle_top_heater_btn, toggle_top_heater_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(toggle_deck_heater_btn, toggle_deck_heater_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(stop_btn, stop_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(pause_btn, pause_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(resume_btn, resume_cb, LV_EVENT_CLICKED, NULL);

  lv_msg_subscribe(MSG_SET_OVEN_MONITOR_DATA, oven_data_ready_cb, NULL);
}

static void scr_unload_cb(lv_event_t *e)
{
  // remove callbacks
  lv_obj_remove_event_cb(temp_inc_btn, temp_inc_cb);
  lv_obj_remove_event_cb(temp_dec_btn, temp_dec_cb);
  lv_obj_remove_event_cb(duration_inc_btn, duration_inc_cb);
  lv_obj_remove_event_cb(duration_dec_btn, duration_dec_cb);

  lv_obj_remove_event_cb(toggle_light_btn, toggle_light_cb);
  lv_obj_remove_event_cb(toggle_fan_btn, toggle_fan_cb);
  lv_obj_remove_event_cb(toggle_grill_btn, toggle_grill_cb);
  lv_obj_remove_event_cb(toggle_top_heater_btn, toggle_top_heater_cb);
  lv_obj_remove_event_cb(toggle_deck_heater_btn, toggle_deck_heater_cb);
  lv_obj_remove_event_cb(stop_btn, stop_cb);
  lv_obj_remove_event_cb(pause_btn, pause_cb);
  lv_obj_remove_event_cb(resume_btn, resume_cb);
}

void init_styles()
{
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

  lv_style_init(&btns_style);
  lv_style_set_radius(&btns_style, 5);
  lv_style_set_width(&btns_style, 30);
  lv_style_set_height(&btns_style, 30);
  lv_style_set_bg_opa(&btns_style, LV_OPA_0);

  lv_style_init(&btns_checked_style);
  lv_style_set_bg_color(&btns_checked_style, lv_color_hex(0xFF8000));
  lv_style_set_bg_opa(&btns_checked_style, LV_OPA_100);

  lv_style_init(&btns_medium_style);
  lv_style_set_radius(&btns_medium_style, 5);
  lv_style_set_width(&btns_medium_style, 40);
  lv_style_set_height(&btns_medium_style, 40);

  lv_style_init(&btns_circle_style);
  lv_style_set_bg_color(&btns_circle_style, lv_color_hex(0xFF8000));
  lv_style_set_bg_opa(&btns_circle_style, LV_OPA_100);
  lv_style_set_radius(&btns_circle_style, 40);
  lv_style_set_width(&btns_circle_style, 40);
  lv_style_set_height(&btns_circle_style, 40);
  lv_style_set_text_font(&btns_circle_style, &lv_font_montserrat_16);
}

void draw_screen_layout()
{
  scr_content = lv_obj_create(baking_scr);
  lv_obj_set_size(scr_content, 240, 320);
  lv_obj_center(scr_content);
  lv_obj_set_flex_flow(scr_content, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_scrollbar_mode(scr_content, LV_SCROLLBAR_MODE_OFF);
  lv_obj_clear_flag(scr_content, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_pad_all(scr_content, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(scr_content, lv_color_hex(0x484848), LV_PART_MAIN);
  lv_obj_set_style_pad_row(scr_content, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  top_bar = lv_obj_create(scr_content);
  lv_obj_set_size(top_bar, LV_PCT(100), 30);
  lv_obj_add_style(top_bar, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  steps_bar = lv_obj_create(scr_content);
  lv_obj_set_flex_flow(steps_bar, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(steps_bar, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_pad_column(steps_bar, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_size(steps_bar, LV_PCT(100), 40);
  lv_obj_add_style(steps_bar, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  temperature_picker = lv_obj_create(scr_content);
  lv_obj_set_flex_flow(temperature_picker, LV_FLEX_FLOW_ROW);
  lv_obj_set_width(temperature_picker, LV_PCT(100));
  lv_obj_set_flex_grow(temperature_picker, 1);
  lv_obj_add_style(temperature_picker, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(temperature_picker, 10, LV_PART_MAIN);
  lv_obj_set_style_pad_right(temperature_picker, 10, LV_PART_MAIN);

  duration_picker = lv_obj_create(scr_content);
  lv_obj_set_flex_flow(duration_picker, LV_FLEX_FLOW_ROW);
  lv_obj_set_width(duration_picker, LV_PCT(100));
  lv_obj_set_flex_grow(duration_picker, 1);
  lv_obj_add_style(duration_picker, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(duration_picker, 10, LV_PART_MAIN);
  lv_obj_set_style_pad_right(duration_picker, 10, LV_PART_MAIN);

  toggles_bar = lv_obj_create(scr_content);
  lv_obj_set_flex_flow(toggles_bar, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(toggles_bar, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_size(toggles_bar, LV_PCT(100), 50);
  lv_obj_add_style(toggles_bar, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  bottom_bar = lv_obj_create(scr_content);
  lv_obj_set_flex_flow(bottom_bar, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(bottom_bar, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_size(bottom_bar, LV_PCT(100), 50);
  lv_obj_add_style(bottom_bar, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void draw_screen_items()
{
  // TITLE CONTAINER
  title_lb = lv_label_create(top_bar);
  lv_label_set_text_fmt(title_lb, "BAKING");
  lv_obj_center(title_lb);

  // STEPS SETUP CONTAINER
  step_name_lb = lv_label_create(steps_bar);
  lv_obj_set_flex_grow(step_name_lb, 1);

  // TEMPERATURE SETUP CONTAINER
  lv_obj_t *temp_dec_btn_container = lv_obj_create(temperature_picker);
  lv_obj_set_height(temp_dec_btn_container, LV_PCT(100));
  lv_obj_set_flex_grow(temp_dec_btn_container, 1);
  lv_obj_add_style(temp_dec_btn_container, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  temp_dec_btn = lv_btn_create(temp_dec_btn_container);
  lv_obj_set_align(temp_dec_btn, LV_ALIGN_CENTER);
  lv_obj_add_style(temp_dec_btn, &btns_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(temp_dec_btn, &btns_circle_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_t *temp_dec_btn_lb = lv_label_create(temp_dec_btn);
  lv_label_set_text_fmt(temp_dec_btn_lb, "+5°C");
  lv_obj_center(temp_dec_btn_lb);

  lv_obj_t *temp_lb_container = lv_obj_create(temperature_picker);
  lv_obj_set_height(temp_lb_container, LV_PCT(100));
  lv_obj_set_flex_grow(temp_lb_container, 3);
  lv_obj_add_style(temp_lb_container, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  curr_temp_lb = lv_label_create(temp_lb_container);
  lv_obj_set_style_text_font(curr_temp_lb, &lv_font_montserrat_44, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_align(curr_temp_lb, LV_ALIGN_CENTER);
  lv_obj_align(curr_temp_lb, LV_ALIGN_CENTER, 0, -10);

  temp_lb = lv_label_create(temp_lb_container);
  lv_obj_set_style_text_font(temp_lb, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_align(temp_lb, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_align(temp_lb, LV_ALIGN_CENTER, 0, 25);

  lv_obj_t *temp_inc_btn_container = lv_obj_create(temperature_picker);
  lv_obj_set_height(temp_inc_btn_container, LV_PCT(100));
  lv_obj_set_flex_grow(temp_inc_btn_container, 1);
  lv_obj_add_style(temp_inc_btn_container, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  temp_inc_btn = lv_btn_create(temp_inc_btn_container);
  lv_obj_add_style(temp_inc_btn, &btns_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(temp_inc_btn, &btns_circle_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_align(temp_inc_btn, LV_ALIGN_CENTER);
  lv_obj_t *temp_inc_btn_lb = lv_label_create(temp_inc_btn);
  lv_label_set_text_fmt(temp_inc_btn_lb, "+5°C");
  lv_obj_center(temp_inc_btn_lb);

  // DURATION SETUP CONTAINER
  lv_obj_t *duration_dec_btn_container = lv_obj_create(duration_picker);
  lv_obj_set_height(duration_dec_btn_container, LV_PCT(100));
  lv_obj_set_flex_grow(duration_dec_btn_container, 1);
  lv_obj_add_style(duration_dec_btn_container, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  duration_dec_btn = lv_btn_create(duration_dec_btn_container);
  lv_obj_set_align(duration_dec_btn, LV_ALIGN_CENTER);
  lv_obj_add_style(duration_dec_btn, &btns_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(duration_dec_btn, &btns_circle_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_t *duration_dec_btn_lb = lv_label_create(duration_dec_btn);
  lv_label_set_text_fmt(duration_dec_btn_lb, "-1m");
  lv_obj_center(duration_dec_btn_lb);

  lv_obj_t *duration_lb_container = lv_obj_create(duration_picker);
  lv_obj_set_height(duration_lb_container, LV_PCT(100));
  lv_obj_set_flex_grow(duration_lb_container, 3);
  lv_obj_add_style(duration_lb_container, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  duration_lb = lv_label_create(duration_lb_container);
  lv_obj_set_style_text_font(duration_lb, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_align(duration_lb, LV_ALIGN_CENTER);

  lv_obj_t *duration_inc_btn_container = lv_obj_create(duration_picker);
  lv_obj_set_height(duration_inc_btn_container, LV_PCT(100));
  lv_obj_set_flex_grow(duration_inc_btn_container, 1);
  lv_obj_add_style(duration_inc_btn_container, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  duration_inc_btn = lv_btn_create(duration_inc_btn_container);
  lv_obj_add_style(duration_inc_btn, &btns_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(duration_inc_btn, &btns_circle_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_align(duration_inc_btn, LV_ALIGN_CENTER);
  lv_obj_t *duration_inc_btn_lb = lv_label_create(duration_inc_btn);
  lv_label_set_text_fmt(duration_inc_btn_lb, "+1m");
  lv_obj_center(duration_inc_btn_lb);

  // FEATURES SETUP CONTAINER
  toggle_light_btn = lv_btn_create(toggles_bar);
  lv_obj_add_style(toggle_light_btn, &btns_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(toggle_light_btn, &btns_medium_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(toggle_light_btn, &btns_checked_style, LV_PART_MAIN | LV_STATE_CHECKED);
  lv_obj_t *light_btn_lb = lv_label_create(toggle_light_btn);
  lv_label_set_text_fmt(light_btn_lb, "L");
  lv_obj_center(light_btn_lb);

  toggle_fan_btn = lv_btn_create(toggles_bar);
  lv_obj_add_style(toggle_fan_btn, &btns_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(toggle_fan_btn, &btns_medium_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(toggle_fan_btn, &btns_checked_style, LV_PART_MAIN | LV_STATE_CHECKED);
  lv_obj_t *fan_btn_lb = lv_label_create(toggle_fan_btn);
  lv_label_set_text_fmt(fan_btn_lb, "F");
  lv_obj_center(fan_btn_lb);

  toggle_grill_btn = lv_btn_create(toggles_bar);
  lv_obj_add_style(toggle_grill_btn, &btns_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(toggle_grill_btn, &btns_medium_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(toggle_grill_btn, &btns_checked_style, LV_PART_MAIN | LV_STATE_CHECKED);
  lv_obj_t *grill_btn_lb = lv_label_create(toggle_grill_btn);
  lv_label_set_text_fmt(grill_btn_lb, "G");
  lv_obj_center(grill_btn_lb);

  toggle_top_heater_btn = lv_btn_create(toggles_bar);
  lv_obj_add_style(toggle_top_heater_btn, &btns_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(toggle_top_heater_btn, &btns_medium_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(toggle_top_heater_btn, &btns_checked_style, LV_PART_MAIN | LV_STATE_CHECKED);
  lv_obj_t *top_heater_btn_lb = lv_label_create(toggle_top_heater_btn);
  lv_label_set_text_fmt(top_heater_btn_lb, "TH");
  lv_obj_center(top_heater_btn_lb);

  toggle_deck_heater_btn = lv_btn_create(toggles_bar);
  lv_obj_add_style(toggle_deck_heater_btn, &btns_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(toggle_deck_heater_btn, &btns_medium_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(toggle_deck_heater_btn, &btns_checked_style, LV_PART_MAIN | LV_STATE_CHECKED);
  lv_obj_t *deck_heater_btn_lb = lv_label_create(toggle_deck_heater_btn);
  lv_label_set_text_fmt(deck_heater_btn_lb, "DH");
  lv_obj_center(deck_heater_btn_lb);

  // CONTROLS
  stop_btn = lv_btn_create(bottom_bar);
  lv_obj_t *stop_btn_lb = lv_label_create(stop_btn);
  lv_label_set_text_fmt(stop_btn_lb, "Stop");

  pause_btn = lv_btn_create(bottom_bar);
  lv_obj_t *pause_btn_lb = lv_label_create(pause_btn);
  lv_label_set_text_fmt(pause_btn_lb, "||");

  resume_btn = lv_btn_create(bottom_bar);
  lv_obj_t *resume_btn_lb = lv_label_create(resume_btn);
  lv_label_set_text_fmt(resume_btn_lb, ">");
}

static void refresh_ui()
{
  oven_data_t *step = &program.steps[program.curr_step_idx];

  // refresh steps bar
  lv_label_set_text_fmt(step_name_lb, "step %d/%d", program.curr_step_idx + 1, program.last_step_idx + 1);

  // refresh temperature and duration labels
  lv_label_set_text_fmt(temp_lb, "SET: %d°C", (int)step->temperature);

  printf("[UI] Oven monitor data ready? %s\n", oven_monitor.ready ? "true" : "false");
  if (!oven_monitor.ready)
  {
    lv_label_set_text_fmt(curr_temp_lb, "--- °C");
    lv_label_set_text_fmt(duration_lb, "-:--");
  }
  else
  {
    lv_label_set_text_fmt(curr_temp_lb, "%d °C", oven_monitor.data.temperature);
    lv_label_set_text_fmt(duration_lb, "%d:%02d", oven_monitor.data.remaining_m / 60, oven_monitor.data.remaining_m % 60);
  }

  if ((int)step->temperature <= BAKING_TEMPERATURE_MIN)
    lv_obj_add_state(temp_dec_btn, LV_STATE_DISABLED);
  else
    lv_obj_clear_state(temp_dec_btn, LV_STATE_DISABLED);

  if ((int)step->temperature >= BAKING_TEMPERATURE_MAX)
    lv_obj_add_state(temp_inc_btn, LV_STATE_DISABLED);
  else
    lv_obj_clear_state(temp_inc_btn, LV_STATE_DISABLED);

  if (oven_monitor.ready)
  {
    if (oven_monitor.data.remaining_m <= BAKING_DURATION_M_MIN)
      lv_obj_add_state(duration_dec_btn, LV_STATE_DISABLED);
    else
      lv_obj_clear_state(duration_dec_btn, LV_STATE_DISABLED);

    if (oven_monitor.data.remaining_m >= BAKING_DURATION_M_MAX)
      lv_obj_add_state(duration_inc_btn, LV_STATE_DISABLED);
    else
      lv_obj_clear_state(duration_inc_btn, LV_STATE_DISABLED);
  }

  // refresh other setup buttons
  if (step->light_on && !lv_obj_has_state(toggle_light_btn, LV_STATE_CHECKED))
    lv_obj_add_state(toggle_light_btn, LV_STATE_CHECKED);
  if (!step->light_on)
    lv_obj_clear_state(toggle_light_btn, LV_STATE_CHECKED);

  if (step->fan_on && !lv_obj_has_state(toggle_fan_btn, LV_STATE_CHECKED))
    lv_obj_add_state(toggle_fan_btn, LV_STATE_CHECKED);
  if (!step->fan_on)
    lv_obj_clear_state(toggle_fan_btn, LV_STATE_CHECKED);

  if (step->grill_on && !lv_obj_has_state(toggle_grill_btn, LV_STATE_CHECKED))
    lv_obj_add_state(toggle_grill_btn, LV_STATE_CHECKED);
  if (!step->grill_on)
    lv_obj_clear_state(toggle_grill_btn, LV_STATE_CHECKED);

  if (step->top_heater_on && !lv_obj_has_state(toggle_top_heater_btn, LV_STATE_CHECKED))
    lv_obj_add_state(toggle_top_heater_btn, LV_STATE_CHECKED);
  if (!step->top_heater_on)
    lv_obj_clear_state(toggle_top_heater_btn, LV_STATE_CHECKED);

  if (step->deck_heater_on && !lv_obj_has_state(toggle_deck_heater_btn, LV_STATE_CHECKED))
    lv_obj_add_state(toggle_deck_heater_btn, LV_STATE_CHECKED);
  if (!step->deck_heater_on)
    lv_obj_clear_state(toggle_deck_heater_btn, LV_STATE_CHECKED);

  if (program.paused)
  {
    btn_set_hidden(pause_btn);
    btn_clear_hidden(resume_btn);
  }
  else
  {
    btn_clear_hidden(pause_btn);
    btn_set_hidden(resume_btn);
  }
}

static void duration_dec_cb(lv_event_t *e)
{
  program.steps[program.curr_step_idx].duration_m -= 1;
  if (program.steps[program.curr_step_idx].duration_m < BAKING_DURATION_M_MIN)
    program.steps[program.curr_step_idx].duration_m = BAKING_DURATION_M_MIN;

  // lv_msg_send(MSG_SET_DURATION_M, &program.steps[program.curr_step_idx].duration_m);

  refresh_ui();
}

static void duration_inc_cb(lv_event_t *e)
{
  program.steps[program.curr_step_idx].duration_m += 1;
  if (program.steps[program.curr_step_idx].duration_m > BAKING_DURATION_M_MIN)
    program.steps[program.curr_step_idx].duration_m = BAKING_DURATION_M_MAX;

  // lv_msg_send(MSG_SET_DURATION_M, &program.steps[program.curr_step_idx].duration_m);

  refresh_ui();
}

static void temp_dec_cb(lv_event_t *e)
{
  program.steps[program.curr_step_idx].temperature -= 5;
  if (program.steps[program.curr_step_idx].temperature < BAKING_TEMPERATURE_MIN)
    program.steps[program.curr_step_idx].temperature = BAKING_TEMPERATURE_MIN;

  lv_msg_send(MSG_SET_TEMPERATURE, &program.steps[program.curr_step_idx].temperature);

  refresh_ui();
}

static void temp_inc_cb(lv_event_t *e)
{
  program.steps[program.curr_step_idx].temperature += 5;
  if (program.steps[program.curr_step_idx].temperature > BAKING_TEMPERATURE_MAX)
    program.steps[program.curr_step_idx].temperature = BAKING_TEMPERATURE_MAX;

  lv_msg_send(MSG_SET_TEMPERATURE, &program.steps[program.curr_step_idx].temperature);

  refresh_ui();
}

static void pause_cb(lv_event_t *e)
{
  LV_UNUSED(e);

  if (!program.paused)
  {
    printf("[UI] Pausing program\n");
    program.paused = true;
    lv_msg_send(MSG_SET_STATUS_PAUSED, NULL);
    refresh_ui();
  }
}

static void resume_cb(lv_event_t *e)
{
  LV_UNUSED(e);

  if (program.paused)
  {
    printf("[UI] Resuming program\n");
    program.paused = false;
    lv_msg_send(MSG_SET_STATUS_RESUMED, NULL);
    refresh_ui();
  }
}

static void stop_cb(lv_event_t *e)
{
  LV_UNUSED(e);
  lv_msg_send(MSG_SET_STATUS_STOPPED, NULL);
}

// makes the button disappear, also removes clickable flag otherwise clicks trigger events
static void btn_set_hidden_soft(lv_obj_t *obj)
{
  lv_obj_set_style_opa(obj, LV_OPA_0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
}
static void btn_clear_hidden_soft(lv_obj_t *obj)
{
  lv_obj_set_style_opa(obj, LV_OPA_100, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
}

static void btn_set_hidden(lv_obj_t *obj)
{
  lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
}
static void btn_clear_hidden(lv_obj_t *obj)
{
  lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
}

void toggle_light_cb(lv_event_t *e)
{
  LV_UNUSED(e);
  printf("[UI] LIGHT CB\n");

  if (program.steps[program.curr_step_idx].light_on)
  {
    printf("[UI] LIGHT OFF\n");
    program.steps[program.curr_step_idx].light_on = false;
  }
  else
  {
    printf("[UI] LIGHT ON\n");
    program.steps[program.curr_step_idx].light_on = true;
  }
  refresh_ui();
}

void toggle_fan_cb(lv_event_t *e)
{
  LV_UNUSED(e);
  if (program.steps[program.curr_step_idx].fan_on)
  {
    program.steps[program.curr_step_idx].fan_on = false;
  }
  else
  {
    program.steps[program.curr_step_idx].fan_on = true;
  }
  refresh_ui();
}

void toggle_grill_cb(lv_event_t *e)
{
  LV_UNUSED(e);
  if (program.steps[program.curr_step_idx].grill_on)
  {
    program.steps[program.curr_step_idx].grill_on = false;
  }
  else
  {
    if (program.steps[program.curr_step_idx].top_heater_on)
      program.steps[program.curr_step_idx].top_heater_on = false;
    program.steps[program.curr_step_idx].grill_on = true;
  }
  refresh_ui();
}

void toggle_top_heater_cb(lv_event_t *e)
{
  LV_UNUSED(e);
  if (program.steps[program.curr_step_idx].top_heater_on)
  {
    program.steps[program.curr_step_idx].top_heater_on = false;
  }
  else
  {
    if (program.steps[program.curr_step_idx].grill_on)
      program.steps[program.curr_step_idx].grill_on = false;
    program.steps[program.curr_step_idx].top_heater_on = true;
  }
  refresh_ui();
}

void toggle_deck_heater_cb(lv_event_t *e)
{
  LV_UNUSED(e);
  if (program.steps[program.curr_step_idx].deck_heater_on)
  {
    program.steps[program.curr_step_idx].deck_heater_on = false;
  }
  else
  {
    program.steps[program.curr_step_idx].deck_heater_on = true;
  }
  refresh_ui();
}
