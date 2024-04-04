#include <lvgl.h>
#include <stdio.h>

/*********************
 *      INCLUDES
 *********************/

#include "../../baking_program.h"
#include "../../main.h"
#include "../components/components.h"
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
static lv_obj_t *steps_btn[BAKING_MAX_STEPS];
// static lv_obj_t *steps_prev_btn;
// static lv_obj_t *steps_next_btn;
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
static void steps_goto(uint16_t, bool);

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
  debug("[UI] Temperature °C  %d\n", curr_step.temperature);
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
  lv_obj_add_style(scr_content, &styles->flex_col_container, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_scrollbar_mode(scr_content, LV_SCROLLBAR_MODE_OFF);
  lv_obj_clear_flag(scr_content, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_size(scr_content, 240, 320);
  lv_obj_center(scr_content);

  top_bar = top_bar_create(scr_content, "BAKING", true, true, back_cb);

  steps_bar = lv_obj_create(scr_content);
  lv_obj_set_height(steps_bar, 20);
  lv_obj_set_flex_align(steps_bar, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_add_style(steps_bar, &styles->flex_row_container, LV_PART_MAIN | LV_STATE_DEFAULT);

  temperature_bar = lv_obj_create(scr_content);
  lv_obj_set_flex_grow(temperature_bar, 2);
  lv_obj_add_style(temperature_bar, &styles->flex_row_container, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_t *temperature_bar_lb = lv_label_create(temperature_bar);
  lv_obj_add_style(temperature_bar_lb, &styles->flex_row_container_title, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_label_set_text(temperature_bar_lb, "TEMPERATURE");
  lv_obj_add_flag(temperature_bar_lb, LV_OBJ_FLAG_FLOATING);

  duration_bar = lv_obj_create(scr_content);
  lv_obj_set_flex_grow(duration_bar, 2);
  lv_obj_add_style(duration_bar, &styles->flex_row_container, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_t *duration_bar_lb = lv_label_create(duration_bar);
  lv_obj_add_style(duration_bar_lb, &styles->flex_row_container_title, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_label_set_text(duration_bar_lb, "DURATION");
  lv_obj_add_flag(duration_bar_lb, LV_OBJ_FLAG_FLOATING);

  toggles_bar = lv_obj_create(scr_content);
  lv_obj_set_height(toggles_bar, 50);
  lv_obj_set_flex_align(toggles_bar, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_add_style(toggles_bar, &styles->flex_row_container, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(toggles_bar, 5, 0);
  lv_obj_set_style_pad_right(toggles_bar, 5, 0);

  bottom_bar = lv_obj_create(scr_content);
  lv_obj_set_flex_align(bottom_bar, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_height(bottom_bar, 50);
  lv_obj_add_style(bottom_bar, &styles->flex_row_container, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_all(bottom_bar, 0, 0);
  lv_obj_set_style_pad_column(bottom_bar, 0, 0);

  LV_IMG_DECLARE(controls_prev);
  LV_IMG_DECLARE(controls_next);
  LV_IMG_DECLARE(controls_play_off);
  LV_IMG_DECLARE(controls_play);

  // // STEPS SETUP CONTAINER
  for (int s = 0; s < BAKING_MAX_STEPS; s++) {
    steps_btn[s] = lv_btn_create(steps_bar);
    lv_obj_add_style(steps_btn[s], &styles->transparent_btn, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(steps_btn[s], &styles->steps_bar_btn, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(steps_btn[s], LV_OBJ_FLAG_HIDDEN);
    lv_obj_t *step_lb = lv_label_create(steps_btn[s]);
    lv_label_set_text_fmt(step_lb, "step %d", s + 1);
    lv_obj_center(step_lb);
  }

  temp_dec_btn = lv_btn_create(temperature_bar);
  lv_obj_set_flex_grow(temp_dec_btn, 1);
  lv_obj_add_style(temp_dec_btn, &styles->transparent_btn, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(temp_dec_btn, &styles->transparent_pressed_btn, LV_STATE_PRESSED);
  lv_obj_add_style(temp_dec_btn, &styles->disabled, LV_STATE_DISABLED);

  lv_obj_t *temp_dec_btn_img = lv_img_create(temp_dec_btn);
  lv_img_set_src(temp_dec_btn_img, &controls_prev);
  lv_obj_center(temp_dec_btn_img);

  lv_obj_t *temp_lb_container = lv_obj_create(temperature_bar);
  lv_obj_set_height(temp_lb_container, LV_PCT(100));
  lv_obj_set_flex_grow(temp_lb_container, 3);
  lv_obj_clear_flag(temp_lb_container, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_add_style(temp_lb_container, &styles->centered_content_container, LV_PART_MAIN | LV_STATE_DEFAULT);

  temp_lb = lv_label_create(temp_lb_container);
  lv_obj_set_align(temp_lb, LV_ALIGN_CENTER);
  lv_obj_set_style_text_font(temp_lb, &lv_font_montserrat_44, LV_PART_MAIN | LV_STATE_DEFAULT);

  temp_inc_btn = lv_btn_create(temperature_bar);
  lv_obj_set_flex_grow(temp_inc_btn, 1);
  lv_obj_add_style(temp_inc_btn, &styles->transparent_btn, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(temp_inc_btn, &styles->transparent_pressed_btn, LV_STATE_PRESSED);
  lv_obj_add_style(temp_inc_btn, &styles->disabled, LV_STATE_DISABLED);

  lv_obj_t *temp_inc_btn_img = lv_img_create(temp_inc_btn);
  lv_img_set_src(temp_inc_btn_img, &controls_next);
  lv_obj_center(temp_inc_btn_img);

  duration_dec_btn = lv_btn_create(duration_bar);
  lv_obj_set_flex_grow(duration_dec_btn, 1);
  lv_obj_add_style(duration_dec_btn, &styles->transparent_btn, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(duration_dec_btn, &styles->transparent_pressed_btn, LV_STATE_PRESSED);
  lv_obj_add_style(duration_dec_btn, &styles->disabled, LV_STATE_DISABLED);

  lv_obj_t *duration_dec_btn_img = lv_img_create(duration_dec_btn);
  lv_img_set_src(duration_dec_btn_img, &controls_prev);
  lv_obj_center(duration_dec_btn_img);

  lv_obj_t *duration_lb_container = lv_obj_create(duration_bar);
  lv_obj_set_height(duration_lb_container, LV_PCT(100));
  lv_obj_set_flex_grow(duration_lb_container, 3);
  lv_obj_clear_flag(duration_lb_container, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_add_style(duration_lb_container, &styles->centered_content_container, LV_PART_MAIN | LV_STATE_DEFAULT);

  duration_lb = lv_label_create(duration_lb_container);
  lv_obj_set_align(duration_lb, LV_ALIGN_CENTER);
  lv_obj_set_style_text_font(duration_lb, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);

  duration_inc_btn = lv_btn_create(duration_bar);
  lv_obj_set_flex_grow(duration_inc_btn, 1);
  lv_obj_add_style(duration_inc_btn, &styles->transparent_btn, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(duration_inc_btn, &styles->transparent_pressed_btn, LV_STATE_PRESSED);
  lv_obj_add_style(duration_inc_btn, &styles->disabled, LV_STATE_DISABLED);

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
  steps_del_btn = lv_btn_create(bottom_bar);
  lv_obj_add_style(steps_del_btn, &styles->transparent_btn, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(steps_del_btn, &styles->btns_bar_btn, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(steps_del_btn, &styles->disabled, LV_PART_MAIN | LV_STATE_DISABLED);

  lv_obj_t *del_step_btn_lb = lv_label_create(steps_del_btn);
  lv_label_set_text_fmt(del_step_btn_lb, "REMOVE\nSTEP");
  lv_obj_set_width(del_step_btn_lb, LV_PCT(100));
  lv_obj_center(del_step_btn_lb);

  steps_add_btn = lv_btn_create(bottom_bar);
  lv_obj_add_style(steps_add_btn, &styles->transparent_btn, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(steps_add_btn, &styles->btns_bar_btn, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(steps_add_btn, &styles->disabled, LV_PART_MAIN | LV_STATE_DISABLED);

  lv_obj_t *add_step_btn_lb = lv_label_create(steps_add_btn);
  lv_label_set_text_fmt(add_step_btn_lb, "ADD\nSTEP");
  lv_obj_set_width(add_step_btn_lb, LV_PCT(100));
  lv_obj_center(add_step_btn_lb);

  start_btn = lv_btn_create(bottom_bar);
  lv_obj_add_style(start_btn, &styles->transparent_btn, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(start_btn, &styles->btns_bar_btn, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(start_btn, &styles->btns_bar_main_btn, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(start_btn, &styles->disabled, LV_PART_MAIN | LV_STATE_DISABLED);

  lv_obj_t *start_btn_lb = lv_label_create(start_btn);
  lv_label_set_text(start_btn_lb, "START");
  lv_obj_set_width(start_btn_lb, LV_PCT(100));
  lv_obj_center(start_btn_lb);

  // ui refresh cbs
  lv_obj_add_event_cb(steps_bar, steps_refresh_cb, LV_EVENT_REFRESH, NULL);
  lv_obj_add_event_cb(temperature_bar, temperature_refresh_cb, LV_EVENT_REFRESH, NULL);
  lv_obj_add_event_cb(duration_bar, duration_refresh_cb, LV_EVENT_REFRESH, NULL);
  lv_obj_add_event_cb(toggles_bar, toggles_refresh_cb, LV_EVENT_REFRESH, NULL);

  event_send_refresh(steps_bar);
  event_send_refresh(temperature_bar);
  event_send_refresh(duration_bar);
  event_send_refresh(toggles_bar);
}

static void steps_refresh_cb(lv_event_t *e) {
  debug("[UI] Steps bar refresh\n");

  for (int s = 0; s < BAKING_MAX_STEPS; s++) {
    if (s < program.steps_count) {
      lv_obj_clear_flag(steps_btn[s], LV_OBJ_FLAG_HIDDEN);
    } else {
      lv_obj_add_flag(steps_btn[s], LV_OBJ_FLAG_HIDDEN);
    }
    if (s == curr_step_idx) {
      lv_obj_add_style(steps_btn[s], &styles->steps_bar_active_btn, 0);
    } else {
      lv_obj_remove_style(steps_btn[s], &styles->steps_bar_active_btn, 0);
    }
  }

  // handle add step
  if (program.steps_count < BAKING_MAX_STEPS) {
    lv_obj_clear_state(steps_add_btn, LV_STATE_DISABLED);
  } else {
    lv_obj_add_state(steps_add_btn, LV_STATE_DISABLED);
  }

  // handle delete step
  if (program.steps_count > 1) {
    lv_obj_clear_state(steps_del_btn, LV_STATE_DISABLED);
  } else {
    lv_obj_add_state(steps_del_btn, LV_STATE_DISABLED);
  }
}

static void temperature_refresh_cb(lv_event_t *e) {
  debug("[UI] Temperature bar refresh\n");
  temp_counter.value = curr_step.temperature;

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

static void steps_goto_cb(lv_event_t *t) {
  lv_obj_t *btn = lv_event_get_target(t);
  for (uint16_t s = 0; s < program.steps_count; s++) {
    if (btn == steps_btn[s]) {
      debug("[UI] Steps going to %d\n", s);
      steps_goto(s, true);
      continue;
    }
  }
}
static void scr_load_cb(lv_event_t *e) {
  for (int s = 0; s < BAKING_MAX_STEPS; s++) {
    lv_obj_add_event_cb(steps_btn[s], steps_goto_cb, LV_EVENT_CLICKED, NULL);
  }
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

  // lv_obj_remove_event_cb(steps_prev_btn, steps_prev_cb);
  // lv_obj_remove_event_cb(steps_next_btn, steps_next_cb);
  lv_obj_remove_event_cb(steps_add_btn, steps_add_cb);
  lv_obj_remove_event_cb(steps_del_btn, steps_del_cb);

  // toggles
  lv_obj_remove_event_cb_with_user_data(toggle_light_btn, toggle_event_cb, &curr_step.light_on);
  lv_obj_remove_event_cb_with_user_data(toggle_fan_btn, toggle_event_cb, &curr_step.fan_on);
  lv_obj_remove_event_cb_with_user_data(toggle_grill_btn, toggle_event_cb, &curr_step.grill_on);
  lv_obj_remove_event_cb_with_user_data(toggle_top_heater_btn, toggle_event_cb, &curr_step.top_heater_on);
  lv_obj_remove_event_cb_with_user_data(toggle_deck_heater_btn, toggle_event_cb, &curr_step.deck_heater_on);

  lv_obj_remove_event_cb(start_btn, start_cb);
}

void steps_add_cb(lv_event_t *e) {
  LV_UNUSED(e);

  if (program.steps_count >= BAKING_MAX_STEPS) {
    return;
  }

  baking_program_add_step(&program);
  steps_goto(curr_step_idx, true);
}

void steps_del_cb(lv_event_t *e) {
  LV_UNUSED(e);

  if (program.steps_count == 1) {
    return;
  }
  debug("[UI] Deleting step idx %d\n", curr_step_idx);
  baking_program_delete_step(&program, curr_step_idx);
  curr_step_idx = curr_step_idx == 0 ? 0 : curr_step_idx - 1;
  steps_goto(curr_step_idx, false);
}

void steps_prev_cb(lv_event_t *e) {
  LV_UNUSED(e);
  steps_goto(curr_step_idx - 1, true);
}

void steps_next_cb(lv_event_t *e) {
  LV_UNUSED(e);
  steps_goto(curr_step_idx + 1, true);
}

void steps_goto(uint16_t step_idx, bool save_prev_step_data) {
  debug("[UI] Goto step idx %d\n", step_idx);

  if (step_idx < 0 || step_idx >= program.steps_count) {
    return;
  }

  uint16_t prev_step_idx = curr_step_idx;
  curr_step_idx = step_idx;

  // save previous step data
  if (save_prev_step_data && prev_step_idx < program.steps_count) {
    debug("[UI] Saving previous step (%d) data", prev_step_idx);
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
    curr_step.temperature = (uint32_t)user_data->value;
    event_send_refresh(temperature_bar);
  }
}

static void temp_inc_cb(lv_event_t *e) {
  if (counter_should_change(e)) {
    counter_t *user_data = (counter_t *)lv_event_get_user_data(e);
    user_data->value += (user_data->value >= BAKING_TEMPERATURE_MAX) ? 0 : 1;
    curr_step.temperature = (uint32_t)user_data->value;
    event_send_refresh(temperature_bar);
  }
}

static void toggle_event_cb(lv_event_t *e) {
  LV_UNUSED(e);
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_VALUE_CHANGED) {
    // toggle the value of the clicked button
    lv_obj_t *obj = lv_event_get_target(e);
    bool *flag = (bool *)lv_event_get_user_data(e);
    *flag = !*flag;

    // handle grill toggle and top heater toggle as radio buttons
    lv_obj_t *target = lv_event_get_target(e);
    if (target == toggle_grill_btn) {
      curr_step.top_heater_on = false;
    }
    if (target == toggle_top_heater_btn){
      curr_step.grill_on = false;
    }

    event_send_refresh(toggles_bar);
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
  debug("Is splash screen null? %s\n", splash_scr == NULL ? "true" : "false");
  lv_scr_load(splash_scr);
  // lv_scr_load_anim(splash_scr, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 300, false);
}
