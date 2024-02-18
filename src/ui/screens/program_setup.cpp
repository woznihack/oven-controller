#include <lvgl.h>
#include <stdio.h>

/*********************
 *      INCLUDES
 *********************/

#include "../../baking_program.h"
#include "../../main.h"
#include "../counter.h"
#include "../messages.h"
#include "screens.h"
#include "styles.h"
#include "toggles.h"

/*********************
 *  GLOBAL VARIABLES
 *********************/
lv_obj_t *program_setup_scr;
extern baking_program_t program;

/*********************
 *  STATIC VARIABLES
 *********************/
static uint16_t curr_step_idx = 0;
static baking_program_step_t curr_step;

static lv_obj_t *title_lb;

/* screen layout */
static lv_obj_t *scr_content;
static lv_obj_t *top_bar;
static lv_obj_t *steps_bar;
static lv_obj_t *temperature_bar;
static lv_obj_t *duration_bar;
static lv_obj_t *toggles_bar;
static lv_obj_t *bottom_bar;

/* steps bar */
static lv_obj_t *step_name_lb;
static lv_obj_t *steps_prev_btn;
static lv_obj_t *steps_next_btn;
static lv_obj_t *steps_add_btn;
static lv_obj_t *steps_del_btn;

/* temperature picker */
static lv_obj_t *temp_lb;
static lv_obj_t *temp_inc_btn;
static lv_obj_t *temp_dec_btn;
static counter_t temp_counter;

/* duration picker */
static lv_obj_t *duration_container;
static lv_obj_t *duration_inc_btn;
static lv_obj_t *duration_dec_btn;
static lv_obj_t *duration_lb;
static counter_t duration_counter;

/* toggles bar */
static lv_obj_t *toggle_light_btn;
static lv_obj_t *toggle_fan_btn;
static lv_obj_t *toggle_grill_btn;
static lv_obj_t *toggle_top_heater_btn;
static lv_obj_t *toggle_deck_heater_btn;

/* bottom bar */
static lv_obj_t *start_btn;
static lv_obj_t *back_btn;

/*********************
 *  STATIC PROTOTYPES
 *********************/
/* main */
static void scr_draw();
static void scr_load_cb(lv_event_t *);
static void scr_unload_cb(lv_event_t *);
static void steps_goto(uint16_t);

/* refresh callbacks */
static void event_send_refresh(lv_obj_t *o);
static void steps_refresh_cb(lv_event_t *);
static void temperature_refresh_cb(lv_event_t *);
static void duration_refresh_cb(lv_event_t *);
static void toggles_refresh_cb(lv_event_t *);

/* button callbacks */
static void steps_add_cb(lv_event_t *);
static void steps_del_cb(lv_event_t *);
static void steps_prev_cb(lv_event_t *);
static void steps_next_cb(lv_event_t *);
static void temp_inc_cb(lv_event_t *e);
static void temp_dec_cb(lv_event_t *e);
static void duration_inc_cb(lv_event_t *e);
static void duration_dec_cb(lv_event_t *e);
static void toggle_event_cb(lv_event_t *e);
static void toggles_top_heater_radio_cb(lv_event_t *e);
static void start_cb(lv_event_t *e);
static void back_cb(lv_event_t *e);

void program_setup_scr_init() {
  program_setup_scr = lv_obj_create(NULL);
  curr_step_idx = 0;
  curr_step = program.steps[curr_step_idx];

  scr_draw();
  lv_obj_add_event_cb(program_setup_scr, scr_load_cb, LV_EVENT_SCREEN_LOADED, NULL);
  lv_obj_add_event_cb(program_setup_scr, scr_unload_cb, LV_EVENT_SCREEN_UNLOADED, NULL);
}

static void debug_step() {
  debug("[UI] ---------- \n");
  debug("[UI] Curr step: \n");
  debug("[UI] Top °C  %d\n", curr_step.top_heater_temperature);
  debug("[UI] Deck °C %d\n", curr_step.deck_heater_temperature);
  debug("[UI] Dur M   %d\n", curr_step.duration_m);
  debug("[UI] light   %s\n", curr_step.light_on ? "on" : "off");
  debug("[UI] fan     %s\n", curr_step.fan_on ? "on" : "off");
  debug("[UI] grill   %s\n", curr_step.grill_on ? "on" : "off");
  debug("[UI] top h   %s\n", curr_step.top_heater_on ? "on" : "off");
  debug("[UI] deck h  %s\n", curr_step.deck_heater_on ? "on" : "off");
}

static void event_send_refresh(lv_obj_t *o) { lv_event_send(o, LV_EVENT_REFRESH, NULL); }

static void scr_draw() {
  // LAYOUT
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

  temperature_bar = lv_obj_create(scr_content);
  lv_obj_set_flex_flow(temperature_bar, LV_FLEX_FLOW_ROW);
  lv_obj_set_width(temperature_bar, LV_PCT(100));
  lv_obj_set_flex_grow(temperature_bar, 1);
  lv_obj_add_style(temperature_bar, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(temperature_bar, 10, LV_PART_MAIN);
  lv_obj_set_style_pad_right(temperature_bar, 10, LV_PART_MAIN);

  duration_bar = lv_obj_create(scr_content);
  lv_obj_set_flex_flow(duration_bar, LV_FLEX_FLOW_ROW);
  lv_obj_set_width(duration_bar, LV_PCT(100));
  lv_obj_set_flex_grow(duration_bar, 1);
  lv_obj_add_style(duration_bar, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(duration_bar, 10, LV_PART_MAIN);
  lv_obj_set_style_pad_right(duration_bar, 10, LV_PART_MAIN);

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

  // TITLE CONTAINER
  title_lb = lv_label_create(top_bar);
  lv_label_set_text_fmt(title_lb, "BAKE CONFIGURATION");
  lv_obj_center(title_lb);

  // STEPS SETUP CONTAINER
  steps_prev_btn = lv_btn_create(steps_bar);
  lv_obj_t *prev_step_btn_lb = lv_label_create(steps_prev_btn);
  lv_label_set_text_fmt(prev_step_btn_lb, "<");
  lv_obj_add_style(steps_prev_btn, &counter_btn_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  step_name_lb = lv_label_create(steps_bar);
  lv_obj_set_flex_grow(step_name_lb, 1);

  steps_next_btn = lv_btn_create(steps_bar);
  lv_obj_t *next_step_btn_lb = lv_label_create(steps_next_btn);
  lv_label_set_text_fmt(next_step_btn_lb, ">");
  lv_obj_add_style(steps_next_btn, &counter_btn_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  steps_add_btn = lv_btn_create(steps_bar);
  lv_obj_t *add_step_btn_lb = lv_label_create(steps_add_btn);
  lv_label_set_text_fmt(add_step_btn_lb, "+");
  lv_obj_add_style(steps_add_btn, &counter_btn_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  steps_del_btn = lv_btn_create(steps_bar);
  lv_obj_t *del_step_btn_lb = lv_label_create(steps_del_btn);
  lv_label_set_text_fmt(del_step_btn_lb, "-");
  lv_obj_add_style(steps_del_btn, &counter_btn_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  // TEMPERATURE SETUP CONTAINER
  LV_IMG_DECLARE(controls_prev);
  LV_IMG_DECLARE(controls_next);
  LV_IMG_DECLARE(controls_play_off);
  LV_IMG_DECLARE(controls_play);

  lv_obj_t *temp_dec_btn_container = lv_obj_create(temperature_bar);
  lv_obj_set_height(temp_dec_btn_container, LV_PCT(100));
  lv_obj_set_flex_grow(temp_dec_btn_container, 1);
  lv_obj_add_style(temp_dec_btn_container, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_t *temp_lb_container = lv_obj_create(temperature_bar);
  lv_obj_set_height(temp_lb_container, LV_PCT(100));
  lv_obj_set_flex_grow(temp_lb_container, 3);
  lv_obj_add_style(temp_lb_container, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_t *temp_inc_btn_container = lv_obj_create(temperature_bar);
  lv_obj_set_height(temp_inc_btn_container, LV_PCT(100));
  lv_obj_set_flex_grow(temp_inc_btn_container, 1);
  lv_obj_add_style(temp_inc_btn_container, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  temp_dec_btn = lv_btn_create(temp_dec_btn_container);
  lv_obj_set_align(temp_dec_btn, LV_ALIGN_CENTER);
  lv_obj_add_style(temp_dec_btn, &counter_btn_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(temp_dec_btn, &counter_btn_pressed_style, LV_STATE_PRESSED);
  // lv_obj_add_style(temp_dec_btn, &counter_btn_disabled_style, LV_STATE_DISABLED);
  lv_obj_t *temp_dec_btn_img = lv_img_create(temp_dec_btn);
  lv_img_set_src(temp_dec_btn_img, &controls_prev);
  lv_obj_center(temp_dec_btn_img);

  temp_lb = lv_label_create(temp_lb_container);
  lv_obj_set_style_text_font(temp_lb, &lv_font_montserrat_44, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_align(temp_lb, LV_ALIGN_CENTER);

  temp_inc_btn = lv_btn_create(temp_inc_btn_container);
  lv_obj_set_align(temp_inc_btn, LV_ALIGN_CENTER);
  lv_obj_add_style(temp_inc_btn, &counter_btn_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(temp_inc_btn, &counter_btn_pressed_style, LV_STATE_PRESSED);
  // lv_obj_add_style(temp_inc_btn, &counter_btn_disabled_style, LV_STATE_DISABLED);
  lv_obj_t *temp_inc_btn_img = lv_img_create(temp_inc_btn);
  lv_img_set_src(temp_inc_btn_img, &controls_next);
  lv_obj_center(temp_inc_btn_img);

  // DURATION SETUP CONTAINER
  lv_obj_t *duration_dec_btn_container = lv_obj_create(duration_bar);
  lv_obj_set_height(duration_dec_btn_container, LV_PCT(100));
  lv_obj_set_flex_grow(duration_dec_btn_container, 1);
  lv_obj_add_style(duration_dec_btn_container, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_t *duration_lb_container = lv_obj_create(duration_bar);
  lv_obj_set_height(duration_lb_container, LV_PCT(100));
  lv_obj_set_flex_grow(duration_lb_container, 3);
  lv_obj_add_style(duration_lb_container, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_t *duration_inc_btn_container = lv_obj_create(duration_bar);
  lv_obj_set_height(duration_inc_btn_container, LV_PCT(100));
  lv_obj_set_flex_grow(duration_inc_btn_container, 1);
  lv_obj_add_style(duration_inc_btn_container, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  duration_dec_btn = lv_btn_create(duration_dec_btn_container);
  lv_obj_set_align(duration_dec_btn, LV_ALIGN_CENTER);
  lv_obj_add_style(duration_dec_btn, &counter_btn_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(duration_dec_btn, &counter_btn_pressed_style, LV_PART_MAIN | LV_STATE_PRESSED);
  lv_obj_add_state(duration_dec_btn, LV_STATE_DISABLED);
  lv_obj_t *duration_dec_btn_img = lv_img_create(duration_dec_btn);
  lv_img_set_src(duration_dec_btn_img, &controls_prev);
  lv_obj_center(duration_dec_btn_img);

  duration_lb = lv_label_create(duration_lb_container);
  lv_obj_set_style_text_font(duration_lb, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_align(duration_lb, LV_ALIGN_CENTER);

  duration_inc_btn = lv_btn_create(duration_inc_btn_container);
  lv_obj_set_align(duration_inc_btn, LV_ALIGN_CENTER);
  lv_obj_add_style(duration_inc_btn, &counter_btn_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(duration_inc_btn, &counter_btn_pressed_style, LV_STATE_PRESSED);
  lv_obj_t *duration_inc_btn_img = lv_img_create(duration_inc_btn);
  lv_img_set_src(duration_inc_btn_img, &controls_next);
  lv_obj_center(duration_inc_btn_img);

  // TOGGLES SETUP
  toggle_light_btn = toggle_create(toggles_bar, TOGGLE_LIGHT);
  toggle_fan_btn = toggle_create(toggles_bar, TOGGLE_FAN);
  toggle_grill_btn = toggle_create(toggles_bar, TOGGLE_GRILL);
  toggle_top_heater_btn = toggle_create(toggles_bar, TOGGLE_TOP_HEATER);
  toggle_deck_heater_btn = toggle_create(toggles_bar, TOGGLE_DECK_HEATER);

  // CONTROLS
  start_btn = lv_imgbtn_create(bottom_bar);
  lv_obj_add_style(start_btn, &toggle_btn_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_imgbtn_set_src(start_btn, LV_IMGBTN_STATE_PRESSED, NULL, &controls_play, NULL);
  lv_imgbtn_set_src(start_btn, LV_IMGBTN_STATE_RELEASED, NULL, &controls_play_off, NULL);
}

static void steps_refresh_cb(lv_event_t *e) {
  debug("[UI] Steps bar refresh\n");
  if (curr_step_idx > 0)
    lv_obj_clear_state(steps_prev_btn, LV_STATE_DISABLED);
  else
    lv_obj_add_state(steps_prev_btn, LV_STATE_DISABLED);

  if (curr_step_idx == program.steps_count - 1) {
    lv_obj_add_flag(steps_next_btn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(steps_add_btn, LV_OBJ_FLAG_HIDDEN);

    if (curr_step_idx == 0)
      lv_obj_add_state(steps_del_btn, LV_STATE_DISABLED);
    else
      lv_obj_clear_state(steps_del_btn, LV_STATE_DISABLED);
  } else {
    lv_obj_clear_flag(steps_next_btn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(steps_add_btn, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_state(steps_del_btn, LV_STATE_DISABLED);
  }

  lv_label_set_text_fmt(step_name_lb, "step %d/%d", curr_step_idx + 1, program.steps_count);
}

static void temperature_refresh_cb(lv_event_t *e) {
  debug("[UI] Temperature bar refresh\n");
  temp_counter.value = curr_step.top_heater_temperature;

  lv_label_set_text_fmt(temp_lb, "%d°C", (int)temp_counter.value);

  if (temp_counter.value <= BAKING_TEMPERATURE_MIN) {
    lv_obj_add_state(temp_dec_btn, LV_STATE_DISABLED);
    lv_obj_set_style_img_opa(lv_obj_get_child(temp_dec_btn, 0), LV_OPA_50, LV_PART_MAIN);
  } else {
    lv_obj_clear_state(temp_dec_btn, LV_STATE_DISABLED);
    lv_obj_set_style_img_opa(lv_obj_get_child(temp_dec_btn, 0), LV_OPA_100, LV_PART_MAIN);
  }

  if (temp_counter.value >= BAKING_TEMPERATURE_MAX) {
    lv_obj_add_state(temp_inc_btn, LV_STATE_DISABLED);
    lv_obj_set_style_img_opa(lv_obj_get_child(temp_inc_btn, 0), LV_OPA_50, LV_PART_MAIN);
  } else {
    lv_obj_clear_state(temp_inc_btn, LV_STATE_DISABLED);
    lv_obj_set_style_img_opa(lv_obj_get_child(temp_inc_btn, 0), LV_OPA_100, LV_PART_MAIN);
  }
}

static void duration_refresh_cb(lv_event_t *e) {
  debug("[UI] Duration bar refresh\n");

  duration_counter.value = curr_step.duration_m;

  lv_label_set_text_fmt(duration_lb, "%d:%02d", (int)duration_counter.value / 60, (int)duration_counter.value % 60);
  if (duration_counter.value <= BAKING_DURATION_M_MIN) {
    lv_obj_add_state(duration_dec_btn, LV_STATE_DISABLED);
    lv_obj_set_style_img_opa(lv_obj_get_child(duration_dec_btn, 0), LV_OPA_50, LV_PART_MAIN);
  } else {
    lv_obj_clear_state(duration_dec_btn, LV_STATE_DISABLED);
    lv_obj_set_style_img_opa(lv_obj_get_child(duration_dec_btn, 0), LV_OPA_100, LV_PART_MAIN);
  }

  if (duration_counter.value >= BAKING_DURATION_M_MAX) {
    lv_obj_add_state(duration_inc_btn, LV_STATE_DISABLED);
    lv_obj_set_style_img_opa(lv_obj_get_child(duration_inc_btn, 0), LV_OPA_50, LV_PART_MAIN);
  } else {
    lv_obj_clear_state(duration_inc_btn, LV_STATE_DISABLED);
    lv_obj_set_style_img_opa(lv_obj_get_child(duration_inc_btn, 0), LV_OPA_100, LV_PART_MAIN);
  }
}

static void toggles_refresh_cb(lv_event_t *e) {
  debug("[UI] Toggles bar refresh\n");
  debug_step();

  lv_imgbtn_set_state(toggle_light_btn, curr_step.light_on ? LV_IMGBTN_STATE_CHECKED_RELEASED : LV_IMGBTN_STATE_RELEASED);
  lv_imgbtn_set_state(toggle_fan_btn, curr_step.fan_on ? LV_IMGBTN_STATE_CHECKED_RELEASED : LV_IMGBTN_STATE_RELEASED);
  lv_imgbtn_set_state(toggle_top_heater_btn, curr_step.top_heater_on ? LV_IMGBTN_STATE_CHECKED_RELEASED : LV_IMGBTN_STATE_RELEASED);
  lv_imgbtn_set_state(toggle_grill_btn, curr_step.grill_on ? LV_IMGBTN_STATE_CHECKED_RELEASED : LV_IMGBTN_STATE_RELEASED);
  lv_imgbtn_set_state(toggle_deck_heater_btn, curr_step.deck_heater_on ? LV_IMGBTN_STATE_CHECKED_RELEASED : LV_IMGBTN_STATE_RELEASED);
}

static void scr_load_cb(lv_event_t *e) {
  lv_obj_add_event_cb(steps_bar, steps_refresh_cb, LV_EVENT_REFRESH, NULL);
  lv_obj_add_event_cb(temperature_bar, temperature_refresh_cb, LV_EVENT_REFRESH, NULL);
  lv_obj_add_event_cb(duration_bar, duration_refresh_cb, LV_EVENT_REFRESH, NULL);
  lv_obj_add_event_cb(toggles_bar, toggles_refresh_cb, LV_EVENT_REFRESH, NULL);

  // add steps bar callbacks
  lv_obj_add_event_cb(steps_prev_btn, steps_prev_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(steps_next_btn, steps_next_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(steps_add_btn, steps_add_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(steps_del_btn, steps_del_cb, LV_EVENT_CLICKED, NULL);

  // add temperature bar callbacks
  counter_add_event_cb(temp_dec_btn, temp_dec_cb, &temp_counter);
  counter_add_event_cb(temp_inc_btn, temp_inc_cb, &temp_counter);

  // add duration bar callbacks
  counter_add_event_cb(duration_dec_btn, duration_dec_cb, &duration_counter);
  counter_add_event_cb(duration_inc_btn, duration_inc_cb, &duration_counter);

  // add toggles callbacks
  lv_obj_add_event_cb(toggle_light_btn, toggle_event_cb, LV_EVENT_VALUE_CHANGED, &curr_step.light_on);
  lv_obj_add_event_cb(toggle_fan_btn, toggle_event_cb, LV_EVENT_VALUE_CHANGED, &curr_step.fan_on);
  lv_obj_add_event_cb(toggle_grill_btn, toggle_event_cb, LV_EVENT_VALUE_CHANGED, &curr_step.grill_on);
  lv_obj_add_event_cb(toggle_top_heater_btn, toggle_event_cb, LV_EVENT_VALUE_CHANGED, &curr_step.top_heater_on);
  lv_obj_add_event_cb(toggle_deck_heater_btn, toggle_event_cb, LV_EVENT_VALUE_CHANGED, &curr_step.deck_heater_on);

  lv_obj_add_event_cb(toggle_grill_btn, toggles_top_heater_radio_cb, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_event_cb(toggle_top_heater_btn, toggles_top_heater_radio_cb, LV_EVENT_VALUE_CHANGED, NULL);

  // add btns callbacks
  lv_obj_add_event_cb(start_btn, start_cb, LV_EVENT_CLICKED, NULL);

  event_send_refresh(steps_bar);
  event_send_refresh(temperature_bar);
  event_send_refresh(duration_bar);
  event_send_refresh(toggles_bar);
}

static void scr_unload_cb(lv_event_t *e) {
  // remove callbacks
  counter_remove_event_cb(temp_dec_btn, temp_dec_cb, &temp_counter);
  counter_remove_event_cb(temp_inc_btn, temp_inc_cb, &temp_counter);
  counter_remove_event_cb(duration_dec_btn, duration_dec_cb, &duration_container);
  counter_remove_event_cb(duration_inc_btn, duration_inc_cb, &duration_container);

  lv_obj_remove_event_cb(steps_prev_btn, steps_prev_cb);
  lv_obj_remove_event_cb(steps_next_btn, steps_next_cb);
  lv_obj_remove_event_cb(steps_add_btn, steps_add_cb);
  lv_obj_remove_event_cb(steps_del_btn, steps_del_cb);

  // toggles
  lv_obj_remove_event_cb_with_user_data(toggle_light_btn, toggle_event_cb, &curr_step.light_on);
  lv_obj_remove_event_cb_with_user_data(toggle_fan_btn, toggle_event_cb, &curr_step.fan_on);
  lv_obj_remove_event_cb_with_user_data(toggle_grill_btn, toggle_event_cb, &curr_step.grill_on);
  lv_obj_remove_event_cb_with_user_data(toggle_top_heater_btn, toggle_event_cb, &curr_step.top_heater_on);
  lv_obj_remove_event_cb_with_user_data(toggle_deck_heater_btn, toggle_event_cb, &curr_step.deck_heater_on);

  lv_obj_remove_event_cb(toggle_grill_btn, toggles_top_heater_radio_cb);
  lv_obj_remove_event_cb(toggle_top_heater_btn, toggles_top_heater_radio_cb);

  lv_obj_remove_event_cb(start_btn, start_cb);
}

void steps_add_cb(lv_event_t *e) {
  LV_UNUSED(e);

  if (program.steps_count >= BAKING_MAX_STEPS) {
    return;
  }

  baking_program_add_step(&program);
  steps_goto(program.steps_count - 1);
}

void steps_del_cb(lv_event_t *e) {
  LV_UNUSED(e);

  if (program.steps_count == 0) {
    return;
  }

  baking_program_delete_last_step(&program);
  steps_goto(program.steps_count - 1);
}

void steps_prev_cb(lv_event_t *e) {
  LV_UNUSED(e);
  steps_goto(curr_step_idx - 1);
}

void steps_next_cb(lv_event_t *e) {
  LV_UNUSED(e);
  steps_goto(curr_step_idx + 1);
}

void steps_goto(uint16_t step_idx) {
  if (step_idx < 0 || step_idx >= program.steps_count) {
    return;
  }

  uint16_t prev_step_idx = curr_step_idx;
  curr_step_idx = step_idx;

  // save previous step data
  if (prev_step_idx != program.steps_count) {
    program.steps[prev_step_idx] = curr_step;
  }

  // load new step data
  curr_step = program.steps[curr_step_idx];

  event_send_refresh(steps_bar);
  event_send_refresh(temperature_bar);
  event_send_refresh(duration_bar);
  event_send_refresh(toggles_bar);
}

static void duration_dec_cb(lv_event_t *e) {
  if (counter_should_change(e)) {
    counter_t *user_data = (counter_t *)lv_event_get_user_data(e);
    user_data->value -= (user_data->value <= BAKING_DURATION_M_MIN) ? 0 : 1;
    curr_step.duration_m = (uint32_t)user_data->value;
    event_send_refresh(duration_bar);
  }
}

static void duration_inc_cb(lv_event_t *e) {
  if (counter_should_change(e)) {
    counter_t *user_data = (counter_t *)lv_event_get_user_data(e);
    user_data->value += (user_data->value >= BAKING_DURATION_M_MAX) ? 0 : 1;
    curr_step.duration_m = (uint32_t)user_data->value;
    event_send_refresh(duration_bar);
  }
}

static void temp_dec_cb(lv_event_t *e) {
  if (counter_should_change(e)) {
    counter_t *user_data = (counter_t *)lv_event_get_user_data(e);
    user_data->value -= (user_data->value <= BAKING_TEMPERATURE_MIN) ? 0 : 1;
    curr_step.top_heater_temperature = (uint32_t)user_data->value;
    curr_step.deck_heater_temperature = (uint32_t)user_data->value;
    event_send_refresh(temperature_bar);
  }
}

static void temp_inc_cb(lv_event_t *e) {
  if (counter_should_change(e)) {
    counter_t *user_data = (counter_t *)lv_event_get_user_data(e);
    user_data->value += (user_data->value >= BAKING_TEMPERATURE_MAX) ? 0 : 1;
    curr_step.top_heater_temperature = (uint32_t)user_data->value;
    curr_step.deck_heater_temperature = (uint32_t)user_data->value;
    event_send_refresh(temperature_bar);
  }
}

static void toggle_event_cb(lv_event_t *e) {
  LV_UNUSED(e);
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_VALUE_CHANGED) {
    lv_obj_t *obj = lv_event_get_target(e);
    bool *flag = (bool *)lv_event_get_user_data(e);
    bool prev_value = *flag ? true : false;
    *flag = lv_obj_has_state(obj, LV_STATE_CHECKED);
    event_send_refresh(toggles_bar);
  }
}

static void toggles_top_heater_radio_cb(lv_event_t *e) {
  lv_obj_t *target = lv_event_get_target(e);

  bool grill_checked = lv_obj_has_state(toggle_grill_btn, LV_STATE_CHECKED);
  bool top_heater_checked = lv_obj_has_state(toggle_top_heater_btn, LV_STATE_CHECKED);

  if (grill_checked && top_heater_checked) {
    if (target == toggle_grill_btn) {
      curr_step.top_heater_on = false;
      lv_imgbtn_set_state(toggle_top_heater_btn, LV_IMGBTN_STATE_RELEASED);
    }
    if (target == toggle_top_heater_btn) {
      curr_step.grill_on = false;
      lv_imgbtn_set_state(toggle_grill_btn, LV_IMGBTN_STATE_RELEASED);
    }
  }
}

static void start_cb(lv_event_t *e) {
  LV_UNUSED(e);

  // save step status
  program.steps[curr_step_idx] = curr_step;

  q_enqueue(oven_control_q, CONTROL_OVEN_SET_BAKING_PROGRAM, &program);
  q_enqueue(oven_control_q, CONTROL_OVEN_START, NULL);

  if (baking_scr == NULL) {
    baking_scr_init();
  }
  lv_scr_load_anim(baking_scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 200, false);
}

static void back_cb(lv_event_t *e) {
  LV_UNUSED(e);
  printf("Is splash screen null? %s\n", splash_scr == NULL ? "true" : "false");
  lv_scr_load(splash_scr);
  // lv_scr_load_anim(splash_scr, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 300, false);
}
