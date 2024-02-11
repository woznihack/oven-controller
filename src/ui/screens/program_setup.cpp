#include <stdio.h>
#include <lvgl.h>

#include "screens.h"
#include "../helpers.h"
#include "../messages.h"
#include "../counter.h"

/* GLOBALS */
lv_obj_t *program_setup_scr;
static uint16_t curr_step_idx = 0;

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
static lv_obj_t *steps_prev_btn;
static lv_obj_t *steps_next_btn;
static lv_obj_t *steps_add_btn;
static lv_obj_t *steps_del_btn;
static void steps_add_cb(lv_event_t *e);
static void steps_del_cb(lv_event_t *e);
static void steps_prev_cb(lv_event_t *e);
static void steps_next_cb(lv_event_t *e);
static void steps_goto(uint16_t step_idx);

/* temperature picker */
static lv_obj_t *temp_lb;
static lv_obj_t *temp_inc_btn;
static lv_obj_t *temp_dec_btn;
static counter_t temp_counter;
static void temp_inc_cb(lv_event_t *e);
static void temp_dec_cb(lv_event_t *e);

/* duration picker */
static lv_obj_t *duration_container;
static lv_obj_t *duration_inc_btn;
static lv_obj_t *duration_dec_btn;
static lv_obj_t *duration_lb;
static counter_t duration_counter;
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
static lv_obj_t *start_btn;
static void start_cb(lv_event_t *e);

void program_setup_scr_init()
{
  program_setup_scr = lv_obj_create(NULL);
  init_styles();
  draw_screen_layout();
  draw_screen_items();
  lv_obj_add_event_cb(program_setup_scr, scr_load_cb, LV_EVENT_SCREEN_LOADED, NULL);
  lv_obj_add_event_cb(program_setup_scr, scr_unload_cb, LV_EVENT_SCREEN_UNLOADED, NULL);
  refresh_ui();
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
  // lv_style_set_bg_color(&btns_circle_style, lv_color_hex(0xFF8000));
  // lv_style_set_bg_opa(&btns_circle_style, LV_OPA_100);
  lv_style_set_radius(&btns_circle_style, 40);
  lv_style_set_width(&btns_circle_style, 40);
  lv_style_set_height(&btns_circle_style, 40);
  lv_style_set_text_font(&btns_circle_style, &lv_font_montserrat_44);
}

void draw_screen_layout()
{
  scr_content = lv_obj_create(program_setup_scr);
  lv_obj_set_size(scr_content, 240, 320);
  lv_obj_center(scr_content);
  lv_obj_set_flex_flow(scr_content, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_style_bg_color(scr_content, lv_color_hex(0x484848), LV_PART_MAIN);
  lv_obj_set_scrollbar_mode(scr_content, LV_SCROLLBAR_MODE_OFF);
  lv_obj_clear_flag(scr_content, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_pad_all(scr_content, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_row(scr_content, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  top_bar = lv_obj_create(scr_content);
  lv_obj_set_size(top_bar, LV_PCT(100), 30);
  lv_obj_add_style(top_bar, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  steps_bar = lv_obj_create(scr_content);
  lv_obj_set_width(steps_bar, LV_PCT(100));
  lv_obj_set_flex_grow(steps_bar, 1);

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
  lv_label_set_text_fmt(title_lb, "BAKE CONFIGURATION");
  lv_obj_center(title_lb);

  // STEPS SETUP CONTAINER
  steps_prev_btn = lv_btn_create(steps_bar);
  lv_obj_t *prev_step_btn_lb = lv_label_create(steps_prev_btn);
  lv_label_set_text_fmt(prev_step_btn_lb, "<");
  lv_obj_add_style(steps_prev_btn, &btns_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  step_name_lb = lv_label_create(steps_bar);
  lv_obj_set_flex_grow(step_name_lb, 1);

  steps_next_btn = lv_btn_create(steps_bar);
  lv_obj_t *next_step_btn_lb = lv_label_create(steps_next_btn);
  lv_label_set_text_fmt(next_step_btn_lb, ">");
  lv_obj_add_style(steps_next_btn, &btns_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  steps_add_btn = lv_btn_create(steps_bar);
  lv_obj_t *add_step_btn_lb = lv_label_create(steps_add_btn);
  lv_label_set_text_fmt(add_step_btn_lb, "+");
  lv_obj_add_style(steps_add_btn, &btns_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  steps_del_btn = lv_btn_create(steps_bar);
  lv_obj_t *del_step_btn_lb = lv_label_create(steps_del_btn);
  lv_label_set_text_fmt(del_step_btn_lb, "-");
  lv_obj_add_style(steps_del_btn, &btns_style, LV_PART_MAIN | LV_STATE_DEFAULT);

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
  lv_label_set_text_fmt(temp_dec_btn_lb, "<");
  lv_obj_center(temp_dec_btn_lb);

  lv_obj_t *temp_lb_container = lv_obj_create(temperature_picker);
  lv_obj_set_height(temp_lb_container, LV_PCT(100));
  lv_obj_set_flex_grow(temp_lb_container, 3);
  lv_obj_add_style(temp_lb_container, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  temp_lb = lv_label_create(temp_lb_container);
  lv_obj_set_style_text_font(temp_lb, &lv_font_montserrat_44, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_align(temp_lb, LV_ALIGN_CENTER);

  lv_obj_t *temp_inc_btn_container = lv_obj_create(temperature_picker);
  lv_obj_set_height(temp_inc_btn_container, LV_PCT(100));
  lv_obj_set_flex_grow(temp_inc_btn_container, 1);
  lv_obj_add_style(temp_inc_btn_container, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  temp_inc_btn = lv_btn_create(temp_inc_btn_container);
  lv_obj_add_style(temp_inc_btn, &btns_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(temp_inc_btn, &btns_circle_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_align(temp_inc_btn, LV_ALIGN_CENTER);
  lv_obj_t *temp_inc_btn_lb = lv_label_create(temp_inc_btn);
  lv_label_set_text_fmt(temp_inc_btn_lb, ">");
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
  lv_label_set_text_fmt(duration_dec_btn_lb, "<");
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
  lv_label_set_text_fmt(duration_inc_btn_lb, ">");
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
  start_btn = lv_btn_create(bottom_bar);
  lv_obj_t *start_btn_lb = lv_label_create(start_btn);
  lv_label_set_text_fmt(start_btn_lb, "Start");
}

static void scr_load_cb(lv_event_t *e)
{
  // add callbacks
  counter_add_event_cb(temp_dec_btn, temp_dec_cb, &temp_counter);
  counter_add_event_cb(temp_inc_btn, temp_inc_cb, &temp_counter);
  counter_add_event_cb(duration_dec_btn, duration_dec_cb, &duration_counter);
  counter_add_event_cb(duration_inc_btn, duration_inc_cb, &duration_counter);

  lv_obj_add_event_cb(steps_prev_btn, steps_prev_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(steps_next_btn, steps_next_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(steps_add_btn, steps_add_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(steps_del_btn, steps_del_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(toggle_light_btn, toggle_light_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(toggle_fan_btn, toggle_fan_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(toggle_grill_btn, toggle_grill_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(toggle_top_heater_btn, toggle_top_heater_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(toggle_deck_heater_btn, toggle_deck_heater_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(start_btn, start_cb, LV_EVENT_CLICKED, NULL);
}

static void scr_unload_cb(lv_event_t *e)
{
  // remove callbacks
  counter_remove_event_cb(temp_dec_btn, temp_dec_cb, &temp_counter);
  counter_remove_event_cb(temp_inc_btn, temp_inc_cb, &temp_counter);
  counter_remove_event_cb(duration_dec_btn, duration_dec_cb, &duration_container);
  counter_remove_event_cb(duration_inc_btn, duration_inc_cb, &duration_container);

  lv_obj_remove_event_cb(steps_prev_btn, steps_prev_cb);
  lv_obj_remove_event_cb(steps_next_btn, steps_next_cb);
  lv_obj_remove_event_cb(steps_add_btn, steps_add_cb);
  lv_obj_remove_event_cb(steps_del_btn, steps_del_cb);
  lv_obj_remove_event_cb(toggle_light_btn, toggle_light_cb);
  lv_obj_remove_event_cb(toggle_fan_btn, toggle_fan_cb);
  lv_obj_remove_event_cb(toggle_grill_btn, toggle_grill_cb);
  lv_obj_remove_event_cb(toggle_top_heater_btn, toggle_top_heater_cb);
  lv_obj_remove_event_cb(toggle_deck_heater_btn, toggle_deck_heater_cb);
  lv_obj_remove_event_cb(start_btn, start_cb);
}

static void refresh_ui()
{
  oven_data_t *step = &program.steps[curr_step_idx];

  // refresh steps bar
  if (curr_step_idx > 0)
    lv_obj_clear_state(steps_prev_btn, LV_STATE_DISABLED);
  else
    lv_obj_add_state(steps_prev_btn, LV_STATE_DISABLED);

  if (curr_step_idx == program.last_step_idx)
  {
    btn_set_hidden(steps_next_btn);
    btn_clear_hidden(steps_add_btn);

    if (curr_step_idx == 0)
      lv_obj_add_state(steps_del_btn, LV_STATE_DISABLED);
    else
      lv_obj_clear_state(steps_del_btn, LV_STATE_DISABLED);
  }
  else
  {
    btn_clear_hidden(steps_next_btn);
    btn_set_hidden(steps_add_btn);
    lv_obj_add_state(steps_del_btn, LV_STATE_DISABLED);
  }
  lv_label_set_text_fmt(step_name_lb, "step %d/%d", curr_step_idx + 1, program.last_step_idx + 1);

  // refresh temperature and duration labels
  temp_counter.value = program.steps[curr_step_idx].temperature;
  duration_counter.value = program.steps[curr_step_idx].duration_m;
  lv_label_set_text_fmt(temp_lb, "%d°C", (int)temp_counter.value);
  lv_label_set_text_fmt(duration_lb, "%d:%02d", (int)duration_counter.value / 60, (int)duration_counter.value % 60);

  if (temp_counter.value <= BAKING_TEMPERATURE_MIN)
    lv_obj_add_state(temp_dec_btn, LV_STATE_DISABLED);
  else
    lv_obj_clear_state(temp_dec_btn, LV_STATE_DISABLED);

  if (temp_counter.value >= BAKING_TEMPERATURE_MAX)
    lv_obj_add_state(temp_inc_btn, LV_STATE_DISABLED);
  else
    lv_obj_clear_state(temp_inc_btn, LV_STATE_DISABLED);

  if (duration_counter.value <= BAKING_DURATION_M_MIN)
    lv_obj_add_state(duration_dec_btn, LV_STATE_DISABLED);
  else
    lv_obj_clear_state(duration_dec_btn, LV_STATE_DISABLED);

  if (duration_counter.value >= BAKING_DURATION_M_MAX)
    lv_obj_add_state(duration_inc_btn, LV_STATE_DISABLED);
  else
    lv_obj_clear_state(duration_inc_btn, LV_STATE_DISABLED);

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
}

static void duration_dec_cb(lv_event_t *e)
{
  if (counter_should_change(e))
  {
    counter_t *user_data = (counter_t *)lv_event_get_user_data(e);
    user_data->value -= (user_data->value <= BAKING_DURATION_M_MIN) ? 0 : 1;
    program.steps[curr_step_idx].duration_m = (uint32_t)user_data->value;
    refresh_ui();
  }
}

static void duration_inc_cb(lv_event_t *e)
{
  if (counter_should_change(e))
  {
    counter_t *user_data = (counter_t *)lv_event_get_user_data(e);
    user_data->value += (user_data->value >= BAKING_DURATION_M_MAX) ? 0 : 1;
    program.steps[curr_step_idx].duration_m = (uint32_t)user_data->value;
    refresh_ui();
  }
}

static void temp_dec_cb(lv_event_t *e)
{
  if (counter_should_change(e))
  {
    counter_t *user_data = (counter_t *)lv_event_get_user_data(e);
    user_data->value -= (user_data->value <= BAKING_TEMPERATURE_MIN) ? 0 : 1;
    program.steps[curr_step_idx].temperature = (uint32_t)user_data->value;
    refresh_ui();
  }
}

static void temp_inc_cb(lv_event_t *e)
{
  if (counter_should_change(e))
  {
    counter_t *user_data = (counter_t *)lv_event_get_user_data(e);
    user_data->value += (user_data->value >= BAKING_TEMPERATURE_MAX) ? 0 : 1;
    program.steps[curr_step_idx].temperature = (uint32_t)user_data->value;
    refresh_ui();
  }
}

void steps_add_cb(lv_event_t *e)
{
  LV_UNUSED(e);

  if (program.last_step_idx + 1 >= BAKING_MAX_STEPS)
  {
    return;
  }

  program_add_step(&program);
  steps_goto(program.last_step_idx);
}

void steps_del_cb(lv_event_t *e)
{
  LV_UNUSED(e);

  if (program.last_step_idx == 0)
  {
    return;
  }

  program_delete_last_step(&program);
  steps_goto(program.last_step_idx);
}

void steps_prev_cb(lv_event_t *e)
{
  LV_UNUSED(e);
  printf("Prev step clicked");
  steps_goto(curr_step_idx - 1);
}

void steps_next_cb(lv_event_t *e)
{
  LV_UNUSED(e);
  printf("Next step clicked");
  steps_goto(curr_step_idx + 1);
}

void steps_goto(uint16_t step_idx)
{
  if (step_idx < 0 || step_idx > program.last_step_idx)
  {
    return;
  }

  printf("[UI] Switching from step %d\n", curr_step_idx);
  printf("[UI] - temperature: %d\n", program.steps[curr_step_idx].temperature);
  printf("[UI] - duration_m: %d\n", program.steps[curr_step_idx].duration_m);

  printf("[UI] .. to step %d\n", step_idx);
  printf("[UI] - temperature: %d\n", program.steps[step_idx].temperature);
  printf("[UI] - duration_m: %d\n", program.steps[step_idx].duration_m);

  curr_step_idx = step_idx;
  refresh_ui();
}

static void start_cb(lv_event_t *e)
{
  LV_UNUSED(e);
  lv_msg_send(MSG_SET_STATUS_STARTED, NULL);
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

  if (program.steps[curr_step_idx].light_on)
  {
    printf("[UI] LIGHT OFF\n");
    program.steps[curr_step_idx].light_on = false;
  }
  else
  {
    printf("[UI] LIGHT ON\n");
    program.steps[curr_step_idx].light_on = true;
  }
  refresh_ui();
}

void toggle_fan_cb(lv_event_t *e)
{
  LV_UNUSED(e);
  if (program.steps[curr_step_idx].fan_on)
  {
    program.steps[curr_step_idx].fan_on = false;
  }
  else
  {
    program.steps[curr_step_idx].fan_on = true;
  }
  refresh_ui();
}

void toggle_grill_cb(lv_event_t *e)
{
  LV_UNUSED(e);
  if (program.steps[curr_step_idx].grill_on)
  {
    program.steps[curr_step_idx].grill_on = false;
  }
  else
  {
    if (program.steps[curr_step_idx].top_heater_on)
      program.steps[curr_step_idx].top_heater_on = false;
    program.steps[curr_step_idx].grill_on = true;
  }
  refresh_ui();
}

void toggle_top_heater_cb(lv_event_t *e)
{
  LV_UNUSED(e);
  if (program.steps[curr_step_idx].top_heater_on)
  {
    program.steps[curr_step_idx].top_heater_on = false;
  }
  else
  {
    if (program.steps[curr_step_idx].grill_on)
      program.steps[curr_step_idx].grill_on = false;
    program.steps[curr_step_idx].top_heater_on = true;
  }
  refresh_ui();
}

void toggle_deck_heater_cb(lv_event_t *e)
{
  LV_UNUSED(e);
  if (program.steps[curr_step_idx].deck_heater_on)
  {
    program.steps[curr_step_idx].deck_heater_on = false;
  }
  else
  {
    program.steps[curr_step_idx].deck_heater_on = true;
  }
  refresh_ui();
}
