#include <lvgl.h>
#include <stdio.h>

#include "../../controller/oven.h"
#include "../../main.h"
#include "../counter.h"
#include "../helpers.h"
#include "../messages.h"
#include "screens.h"

/* GLOBALS */
lv_obj_t *baking_scr;
static oven_monitor_data_t monitor_data;
bool monitor_data_valid = false;
bool baking_started = false;
bool baking_complete = false;

/* main */
static lv_timer_t *screen_update_timer;
static lv_obj_t *title_lb;
static void init_styles();
static void draw_screen_layout();
static void draw_screen_items();
static void scr_load_cb(lv_event_t *e);
static void scr_unload_cb(lv_event_t *e);
static void handle_oven_monitor_q(lv_timer_t *timer);
static void update_monitor_data_cb(oven_monitor_data_t);
static void refresh_ui_loading();
static void refresh_ui_baking_completed();
static void refresh_ui_baking_in_progress();
static void refresh_ui();

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
static lv_obj_t *temperature_monitor;
static lv_obj_t *duration_picker;
static lv_obj_t *toggles_bar;
static lv_obj_t *loading_spinner;
static lv_obj_t *bottom_bar;
static lv_obj_t *completed_panel;

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
static lv_obj_t *remaining_m_lb;
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

/* completed panel */
static lv_obj_t *home_btn;
static void home_cb(lv_event_t *e);

static void dump_monitor_data(oven_monitor_data_t data) {
  debug("[UI] --- MONITOR DATA ---\n");
  debug("[UI] current_phase = %s\n", oven_state_strings[data.current_state]);
  debug("[UI] top heater temperature = %d\n", data.top_heater_temperature);
  debug("[UI] deck heater temperature = %d\n", data.deck_heater_temperature);
  debug("[UI] remaining_m = %d\n", data.remaining_m);
}

void baking_scr_init() {
  baking_scr = lv_obj_create(NULL);
  init_styles();
  draw_screen_layout();
  draw_screen_items();
  refresh_ui_loading();

  lv_obj_add_event_cb(baking_scr, scr_load_cb, LV_EVENT_SCREEN_LOADED, NULL);
  lv_obj_add_event_cb(baking_scr, scr_unload_cb, LV_EVENT_SCREEN_UNLOADED, NULL);
}

static void scr_load_cb(lv_event_t *e) {
  // reinitialize screen
  baking_started = false;
  baking_complete = false;

  // init monitor_data
  monitor_data.current_state = OVEN_S_IDLE;
  monitor_data.avg_heater_temperature = -1;
  monitor_data.top_heater_temperature = -1;
  monitor_data.deck_heater_temperature = -1;
  monitor_data.remaining_m = -1;

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
  lv_obj_add_event_cb(home_btn, home_cb, LV_EVENT_CLICKED, NULL);

  // set timer
  screen_update_timer = lv_timer_create(handle_oven_monitor_q, 100, NULL);
  refresh_ui_loading();
}

static void scr_unload_cb(lv_event_t *e) {
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
  lv_obj_remove_event_cb(home_btn, home_cb);

  // delete timer
  lv_timer_del(screen_update_timer);
}

void init_styles() {
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

void draw_screen_layout() {
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

  temperature_monitor = lv_obj_create(scr_content);
  lv_obj_set_flex_flow(temperature_monitor, LV_FLEX_FLOW_ROW);
  lv_obj_set_width(temperature_monitor, LV_PCT(100));
  lv_obj_set_flex_grow(temperature_monitor, 1);
  lv_obj_add_style(temperature_monitor, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(temperature_monitor, 10, LV_PART_MAIN);
  lv_obj_set_style_pad_right(temperature_monitor, 10, LV_PART_MAIN);

  duration_picker = lv_obj_create(scr_content);
  lv_obj_set_flex_flow(duration_picker, LV_FLEX_FLOW_ROW);
  lv_obj_set_width(duration_picker, LV_PCT(100));
  lv_obj_set_flex_grow(duration_picker, 1);
  lv_obj_add_style(duration_picker, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(duration_picker, 10, LV_PART_MAIN);
  lv_obj_set_style_pad_right(duration_picker, 10, LV_PART_MAIN);

  loading_spinner = lv_obj_create(scr_content);
  lv_obj_set_flex_flow(loading_spinner, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(loading_spinner, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_width(loading_spinner, LV_PCT(100));
  lv_obj_set_flex_grow(loading_spinner, 1);
  lv_obj_add_style(loading_spinner, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_t *loading_lb = lv_label_create(loading_spinner);
  lv_label_set_text_static(loading_lb, "LOADING...");
  lv_obj_center(loading_lb);

  completed_panel = lv_obj_create(scr_content);
  lv_obj_set_flex_flow(completed_panel, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(completed_panel, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_width(completed_panel, LV_PCT(100));
  lv_obj_set_flex_grow(completed_panel, 1);
  lv_obj_add_style(completed_panel, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);

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
void draw_screen_items() {
  // TITLE CONTAINER
  title_lb = lv_label_create(top_bar);
  lv_label_set_text_fmt(title_lb, "BAKING");
  lv_obj_center(title_lb);

  // STEPS SETUP CONTAINER
  step_name_lb = lv_label_create(steps_bar);
  lv_obj_set_flex_grow(step_name_lb, 1);

  // TEMPERATURE SETUP CONTAINER
  lv_obj_t *temp_dec_btn_container = lv_obj_create(temperature_monitor);
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

  lv_obj_t *temp_lb_container = lv_obj_create(temperature_monitor);
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

  lv_obj_t *temp_inc_btn_container = lv_obj_create(temperature_monitor);
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

  lv_obj_t *remaining_m_lb_container = lv_obj_create(duration_picker);
  lv_obj_set_height(remaining_m_lb_container, LV_PCT(100));
  lv_obj_set_flex_grow(remaining_m_lb_container, 3);
  lv_obj_add_style(remaining_m_lb_container, &container_style, LV_PART_MAIN | LV_STATE_DEFAULT);
  remaining_m_lb = lv_label_create(remaining_m_lb_container);
  lv_obj_set_style_text_font(remaining_m_lb, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_align(remaining_m_lb, LV_ALIGN_CENTER);

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

  // completed panel
  lv_obj_t *completed_panel_lb = lv_label_create(completed_panel);
  lv_label_set_text_static(completed_panel_lb, "Baking is complete!");
  lv_obj_center(completed_panel_lb);

  // lv_obj_t *restart_btn = lv_btn_create(completed_panel);
  // lv_obj_t *restart_btn_lb = lv_label_create(restart_btn);
  // lv_label_set_text_static(restart_btn_lb, "Restart!");

  home_btn = lv_btn_create(completed_panel);
  lv_obj_t *home_btn_lb = lv_label_create(home_btn);
  lv_label_set_text_static(home_btn_lb, "Back to main screen");
}

static void refresh_ui_baking_completed() {
  // hides everything but the spinner
  lv_obj_add_flag(steps_bar, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(temperature_monitor, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(duration_picker, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(toggles_bar, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(loading_spinner, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(bottom_bar, LV_OBJ_FLAG_HIDDEN);

  lv_obj_clear_flag(completed_panel, LV_OBJ_FLAG_HIDDEN);
}

static void refresh_ui_baking_in_progress() {
  lv_label_set_text_fmt(step_name_lb, "%s", oven_state_strings[monitor_data.current_state]);
  if (monitor_data.current_state == OVEN_S_BAKING && monitor_data.program_steps_total > 1) {
    lv_label_set_text_fmt(step_name_lb, "baking %d/%d", monitor_data.program_step_current, monitor_data.program_steps_total);
  }

  lv_label_set_text_fmt(temp_lb, "SET: %d°C", monitor_data.config_temperature);
  lv_label_set_text_fmt(remaining_m_lb, "%d:%02d", monitor_data.remaining_m / 60, monitor_data.remaining_m % 60);
  lv_label_set_text_fmt(curr_temp_lb, "%d °C", monitor_data.avg_heater_temperature);

  // refresh temperature button state
  if (monitor_data.avg_heater_temperature <= BAKING_TEMPERATURE_MIN)
    lv_obj_add_state(temp_dec_btn, LV_STATE_DISABLED);
  else
    lv_obj_clear_state(temp_dec_btn, LV_STATE_DISABLED);

  if (monitor_data.avg_heater_temperature >= BAKING_TEMPERATURE_MAX)
    lv_obj_add_state(temp_inc_btn, LV_STATE_DISABLED);
  else
    lv_obj_clear_state(temp_inc_btn, LV_STATE_DISABLED);

  // refresh duration button state
  if (monitor_data.remaining_m <= BAKING_DURATION_M_MIN)
    lv_obj_add_state(duration_dec_btn, LV_STATE_DISABLED);
  else
    lv_obj_clear_state(duration_dec_btn, LV_STATE_DISABLED);

  if (monitor_data.remaining_m >= BAKING_DURATION_M_MAX)
    lv_obj_add_state(duration_inc_btn, LV_STATE_DISABLED);
  else
    lv_obj_clear_state(duration_inc_btn, LV_STATE_DISABLED);

  // refresh other setup buttons
  if (monitor_data.light_on && !lv_obj_has_state(toggle_light_btn, LV_STATE_CHECKED)) lv_obj_add_state(toggle_light_btn, LV_STATE_CHECKED);
  if (!monitor_data.light_on) lv_obj_clear_state(toggle_light_btn, LV_STATE_CHECKED);

  if (monitor_data.fan_on && !lv_obj_has_state(toggle_fan_btn, LV_STATE_CHECKED)) lv_obj_add_state(toggle_fan_btn, LV_STATE_CHECKED);
  if (!monitor_data.fan_on) lv_obj_clear_state(toggle_fan_btn, LV_STATE_CHECKED);

  if (monitor_data.grill_on && !lv_obj_has_state(toggle_grill_btn, LV_STATE_CHECKED)) lv_obj_add_state(toggle_grill_btn, LV_STATE_CHECKED);
  if (!monitor_data.grill_on) lv_obj_clear_state(toggle_grill_btn, LV_STATE_CHECKED);

  if (monitor_data.top_heater_on && !lv_obj_has_state(toggle_top_heater_btn, LV_STATE_CHECKED)) lv_obj_add_state(toggle_top_heater_btn, LV_STATE_CHECKED);
  if (!monitor_data.top_heater_on) lv_obj_clear_state(toggle_top_heater_btn, LV_STATE_CHECKED);

  if (monitor_data.deck_heater_on && !lv_obj_has_state(toggle_deck_heater_btn, LV_STATE_CHECKED)) lv_obj_add_state(toggle_deck_heater_btn, LV_STATE_CHECKED);
  if (!monitor_data.deck_heater_on) lv_obj_clear_state(toggle_deck_heater_btn, LV_STATE_CHECKED);

  if (!monitor_data_valid) {
    monitor_data_valid = true;
    if (monitor_data.current_state == OVEN_S_PREHEATING) {
      lv_obj_clear_flag(steps_bar, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(temperature_monitor, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(toggles_bar, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(temp_lb, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(bottom_bar, LV_OBJ_FLAG_HIDDEN);
    } else if (monitor_data.current_state == OVEN_S_BAKING) {
      lv_obj_clear_flag(steps_bar, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(temperature_monitor, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(duration_picker, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(toggles_bar, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(temp_lb, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(bottom_bar, LV_OBJ_FLAG_HIDDEN);
    } else if (monitor_data.current_state == OVEN_S_COOLDOWN) {
      lv_obj_clear_flag(steps_bar, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(temperature_monitor, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(temp_dec_btn, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(temp_inc_btn, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(temp_lb, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(bottom_bar, LV_OBJ_FLAG_HIDDEN);
    }
    lv_obj_add_flag(loading_spinner, LV_OBJ_FLAG_HIDDEN);
  }
}

static void refresh_ui_loading() {
  // hides everything but the spinner
  lv_obj_add_flag(steps_bar, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(temperature_monitor, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(duration_picker, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(toggles_bar, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(bottom_bar, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(completed_panel, LV_OBJ_FLAG_HIDDEN);

  // shows the spinner
  lv_obj_clear_flag(loading_spinner, LV_OBJ_FLAG_HIDDEN);
}

void handle_oven_monitor_q(lv_timer_t *timer) {
  q_node_t data;
  while (q_dequeue(oven_monitor_q, &data)) {
    if (data.event == MONITOR_UPDATE_OVEN_DATA) {
      update_monitor_data_cb(*(oven_monitor_data_t *)data.payload);
    }
  }
}

static void update_monitor_data_cb(oven_monitor_data_t data) {
  oven_monitor_data_t prev_data = monitor_data;
  monitor_data = data;
  // dump_monitor_data(monitor_data);

  // on a phase change, we show the loading spinner and invalidate data
  if (prev_data.current_state != data.current_state) {
    monitor_data_valid = false;
    if (prev_data.current_state == OVEN_S_COOLDOWN && data.current_state == OVEN_S_IDLE) {
      baking_complete = true;
    } else {
      baking_started = true;
      refresh_ui_loading();
    }
  } else if (baking_complete) {
    refresh_ui_baking_completed();
  } else if (baking_started) {
    refresh_ui_baking_in_progress();
  }
}

static void refresh_ui() {
  debug("[UI] refreshing baking screen\n");
  debug("[UI] monitor data ready? %s\n", monitor_data_valid ? "true" : "false");

  // if (program.paused) {
  //   lv_obj_add_flag(pause_btn, LV_OBJ_FLAG_HIDDEN);
  //   lv_obj_clear_flag(resume_btn, LV_OBJ_FLAG_HIDDEN);
  // } else {
  //   lv_obj_clear_flag(pause_btn, LV_OBJ_FLAG_HIDDEN);
  //   lv_obj_add_flag(resume_btn, LV_OBJ_FLAG_HIDDEN);
  // }
}

static void duration_dec_cb(lv_event_t *e) {
  uint32_t new_duration = MAX(monitor_data.config_duration_m - 1, BAKING_DURATION_M_MIN);
  monitor_data.config_duration_m = new_duration;
  q_enqueue(oven_control_q, CONTROL_OVEN_SET_DURATION_M, &new_duration);
}

static void duration_inc_cb(lv_event_t *e) {
  uint32_t new_duration = MIN(monitor_data.config_duration_m + 1, BAKING_DURATION_M_MAX);
  monitor_data.config_duration_m = new_duration;
  q_enqueue(oven_control_q, CONTROL_OVEN_SET_DURATION_M, &new_duration);
}

static void temp_dec_cb(lv_event_t *e) {
  uint32_t new_temperature = MAX(monitor_data.config_temperature - 5, BAKING_TEMPERATURE_MIN);
  monitor_data.config_temperature = new_temperature;
  q_enqueue(oven_control_q, CONTROL_OVEN_SET_TOP_HEATER_TEMP, &new_temperature);
  q_enqueue(oven_control_q, CONTROL_OVEN_SET_DECK_HEATER_TEMP, &new_temperature);
}

static void temp_inc_cb(lv_event_t *e) {
  uint32_t new_temperature = MIN(monitor_data.config_temperature + 5, BAKING_TEMPERATURE_MAX);
  monitor_data.config_temperature = new_temperature;
  q_enqueue(oven_control_q, CONTROL_OVEN_SET_TOP_HEATER_TEMP, &new_temperature);
  q_enqueue(oven_control_q, CONTROL_OVEN_SET_DECK_HEATER_TEMP, &new_temperature);
}

static void pause_cb(lv_event_t *e) { LV_UNUSED(e); }

static void resume_cb(lv_event_t *e) { LV_UNUSED(e); }

static void stop_cb(lv_event_t *e) {
  LV_UNUSED(e);
  lv_msg_send(MSG_SET_STATUS_STOPPED, NULL);
}

static void home_cb(lv_event_t *e) {
  LV_UNUSED(e);

  if (program_setup_scr == NULL) {
    program_setup_scr_init();
  }
  lv_scr_load_anim(program_setup_scr, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 200, false);
}

void toggle_light_cb(lv_event_t *e) {
  LV_UNUSED(e);
  printf("[UI] LIGHT CB\n");

  if (monitor_data.light_on) {
    printf("[UI] LIGHT OFF\n");
    monitor_data.light_on = false;
  } else {
    printf("[UI] LIGHT ON\n");
    monitor_data.light_on = true;
  }
  refresh_ui();
}

void toggle_fan_cb(lv_event_t *e) {
  LV_UNUSED(e);
  if (monitor_data.fan_on) {
    monitor_data.fan_on = false;
  } else {
    monitor_data.fan_on = true;
  }
  refresh_ui();
}

void toggle_grill_cb(lv_event_t *e) {
  LV_UNUSED(e);
  if (monitor_data.grill_on) {
    monitor_data.grill_on = false;
  } else {
    if (monitor_data.top_heater_on) monitor_data.top_heater_on = false;
    monitor_data.grill_on = true;
  }
  refresh_ui();
}

void toggle_top_heater_cb(lv_event_t *e) {
  LV_UNUSED(e);
  if (monitor_data.top_heater_on) {
    monitor_data.top_heater_on = false;
  } else {
    if (monitor_data.grill_on) monitor_data.grill_on = false;
    monitor_data.top_heater_on = true;
  }
  refresh_ui();
}

void toggle_deck_heater_cb(lv_event_t *e) {
  LV_UNUSED(e);
  if (monitor_data.deck_heater_on) {
    monitor_data.deck_heater_on = false;
  } else {
    monitor_data.deck_heater_on = true;
  }
  refresh_ui();
}
