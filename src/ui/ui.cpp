#include <stdio.h>
#include <lvgl.h>
#include <stdio.h>

#include "ui.h"
#include "counter.h"

#define TEMPERATURE_MAX_VALUE 200
#define TEMPERATURE_MIN_VALUE 50

#define MINUTES_MAX_VALUE 600
#define MINUTES_MIN_VALUE 1

// global variables
lv_obj_t * splash_screen;
lv_obj_t * main_screen;
lv_obj_t * temp_msgbox_lb;
lv_obj_t * time_msgbox_h_lb;
lv_obj_t * time_msgbox_m_lb;
lv_obj_t * temp_lb;
lv_obj_t * time_h_lb;
lv_obj_t * time_m_lb;
lv_obj_t * time_separator_lb;
lv_obj_t * grid_btns[8];

lv_event_cb_t grid_btns_cb[8];
lv_anim_enable_t time_separator_anim_status = LV_ANIM_OFF;

// styles
static lv_style_t main_style;
static lv_style_t alpha_bg_style;

char programName[] = "manual program";
bool started = false;
int temperature = 10;
int remainingMinutes = 10;
int setTemperature = 10;
int setDurationMinutes = 10;

counter_user_data temp_data = counter_data_create(setTemperature);
counter_user_data time_data = counter_data_create(setDurationMinutes);

// prototypes
void splash_screen_init();
void main_screen_init();
void main_screen_update(lv_timer_t * timer);
void set_grid_btn_cb(int idx, lv_event_cb_t cb);
void start_alpha_animation(lv_anim_enable_t * status, lv_obj_t * obj, lv_anim_exec_xcb_t cb, int32_t duration, int32_t delay);
void stop_alpha_animation(lv_anim_enable_t * status, lv_obj_t * obj, lv_anim_exec_xcb_t cb);
static void anim_alpha_cb(void * var, int32_t v);
void styles_init();
void handle_queue();

void temp_msgbox_open(lv_event_t *);
void temp_msgbox_btn_cb(lv_event_t *);
void temp_msgbox_dec_cb(lv_event_t *);
void temp_msgbox_inc_cb(lv_event_t *);

void time_msgbox_open(lv_event_t *);
void time_msgbox_btn_cb(lv_event_t *);
void time_msgbox_inc_cb(lv_event_t *);
void time_msgbox_dec_cb(lv_event_t *);
void time_update_lb(int, lv_obj_t *, lv_obj_t *);

void start_program(lv_event_t * t){
  double payload = 4.5;
  q_enqueue(oven_queue, OVEN_START, NULL);
}
void end_program(lv_event_t * t){
  q_enqueue(oven_queue, OVEN_STOP, NULL);
}

void ui_init(void){
  splash_screen_init();
  main_screen_init();

  lv_disp_load_scr(splash_screen);
  lv_scr_load_anim(main_screen, LV_SCR_LOAD_ANIM_FADE_ON, 1000, 1000, true);
  lv_timer_t *main_screen_update_timer = lv_timer_create(main_screen_update, 100, NULL);
}

void set_grid_btn_cb(int idx, lv_event_cb_t cb){
  lv_obj_remove_event_cb(grid_btns[idx], grid_btns_cb[idx]);
  grid_btns_cb[idx] = cb;
  lv_obj_add_event_cb(grid_btns[idx], grid_btns_cb[idx], LV_EVENT_CLICKED, NULL);
}

void styles_init(){
  lv_style_init(&main_style);
  lv_style_set_border_width(&main_style, 0);
  lv_style_set_outline_width(&main_style, 1);
  lv_style_set_pad_all(&main_style, 0);
  lv_style_set_pad_row(&main_style, 0);
  lv_style_set_pad_column(&main_style, 0);
  lv_style_set_radius(&main_style, 0);

  lv_style_init(&alpha_bg_style);
  lv_style_set_bg_opa(&alpha_bg_style, LV_OPA_0);
}

void splash_screen_init(){
  splash_screen = lv_obj_create(NULL);
  lv_obj_clear_flag( splash_screen, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
  lv_obj_set_style_bg_color(splash_screen, lv_color_hex(0x3E3E3E), LV_PART_MAIN | LV_STATE_DEFAULT );
  lv_obj_set_style_bg_opa(splash_screen, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

  LV_IMG_DECLARE(logo)
  lv_obj_t * logo_img = lv_img_create(splash_screen);
  lv_img_set_src(logo_img, &logo);
  lv_obj_align(logo_img, LV_ALIGN_CENTER, 0, -20);
}

void main_screen_init(){

  styles_init();

  main_screen = lv_obj_create(NULL);
  lv_obj_add_style(main_screen, &main_style, 0);
  lv_obj_set_scrollbar_mode(main_screen, LV_SCROLLBAR_MODE_OFF);
  lv_obj_clear_flag( main_screen, LV_OBJ_FLAG_SCROLLABLE );

  lv_obj_t *main_panel = lv_obj_create(main_screen);
  lv_obj_set_scrollbar_mode(main_panel, LV_SCROLLBAR_MODE_OFF);
  lv_obj_clear_flag( main_panel, LV_OBJ_FLAG_SCROLLABLE );
  lv_obj_set_size(main_panel, 240, 320);
  lv_obj_align(main_panel, LV_ALIGN_TOP_LEFT, 0, 0);
  lv_obj_add_style(main_panel, &main_style, 0);

  static lv_coord_t main_col_dsc[] = {240, LV_GRID_TEMPLATE_LAST};
  static lv_coord_t main_row_dsc[] = {100, 100, 120, LV_GRID_TEMPLATE_LAST};
  lv_obj_set_grid_dsc_array(main_panel, main_col_dsc, main_row_dsc);

  lv_obj_t *top_panel = lv_obj_create(main_panel);
  lv_obj_t *middle_panel = lv_obj_create(main_panel);
  lv_obj_t *bot_panel = lv_obj_create(main_panel);

  lv_obj_set_grid_cell(top_panel,    LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
  lv_obj_set_grid_cell(middle_panel, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
  lv_obj_set_grid_cell(bot_panel,    LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 2, 1);

  lv_obj_add_style(top_panel, &main_style, 0);
  lv_obj_add_style(middle_panel, &main_style, 0);
  lv_obj_add_style(bot_panel, &main_style, 0);

  /* top panel content */
  lv_obj_t * program_name = lv_label_create(top_panel);
  lv_obj_add_style(program_name, &main_style, 0);
  lv_obj_add_style(program_name, &alpha_bg_style, 0);
  lv_label_set_text_fmt(program_name, "%s", programName);

  lv_obj_t * time_container = lv_obj_create(top_panel);
  lv_obj_center(time_container);
  lv_obj_add_style(time_container, &main_style, 0);
  lv_obj_add_style(time_container, &alpha_bg_style, 0);
  lv_obj_clear_flag(time_container, LV_OBJ_FLAG_SCROLLABLE );
  lv_obj_set_size(time_container, 70, 32);
  lv_obj_set_style_outline_width(time_container, 1, LV_STATE_DEFAULT);

  time_h_lb = lv_label_create(time_container);
  lv_obj_set_style_text_font(time_h_lb, &lv_font_montserrat_32, LV_PART_MAIN| LV_STATE_DEFAULT);
  lv_obj_set_style_text_align(time_h_lb, LV_TEXT_ALIGN_RIGHT, 0);

  time_separator_lb = lv_label_create(time_container);
  lv_label_set_text_fmt(time_separator_lb, ":");
  lv_obj_set_style_text_font(time_separator_lb, &lv_font_montserrat_32, LV_PART_MAIN| LV_STATE_DEFAULT);

  time_m_lb = lv_label_create(time_container);
  lv_obj_set_style_text_font(time_m_lb, &lv_font_montserrat_32, LV_PART_MAIN| LV_STATE_DEFAULT);
  lv_obj_set_style_text_align(time_h_lb, LV_TEXT_ALIGN_LEFT, 0);

  time_update_lb(setDurationMinutes, time_h_lb, time_m_lb);

  lv_obj_align(time_m_lb, LV_ALIGN_RIGHT_MID, 0, 0);
  lv_obj_align_to(time_separator_lb, time_m_lb, LV_ALIGN_OUT_LEFT_MID, 0, 0);
  lv_obj_align_to(time_h_lb, time_separator_lb, LV_ALIGN_OUT_LEFT_MID, 0, 0);

  /* middle panel content */
  temp_lb = lv_label_create(middle_panel);
  lv_obj_center(temp_lb);
  lv_label_set_text_fmt(temp_lb, "%d", setTemperature);
  lv_obj_set_style_text_font(temp_lb, &lv_font_montserrat_48, LV_PART_MAIN| LV_STATE_DEFAULT);

  /* bottom panel content */
  static lv_coord_t col_dsc[] = {60, 60, 60, 60, LV_GRID_TEMPLATE_LAST};
  static lv_coord_t row_dsc[] = {60, 60, LV_GRID_TEMPLATE_LAST};
  lv_obj_set_grid_dsc_array(bot_panel, col_dsc, row_dsc);
  lv_obj_set_scrollbar_mode(bot_panel, LV_SCROLLBAR_MODE_OFF);

  lv_obj_t * label;
  lv_obj_t * obj;
  lv_obj_t * btn;
  uint32_t i;
  for(i = 0; i < 8; i++) {
      uint8_t col = i % 4;
      uint8_t row = i / 4;

      obj = lv_obj_create(bot_panel);
      lv_obj_add_style(obj, &main_style, 0);
      lv_obj_add_style(obj, &alpha_bg_style, 0);
      lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);    /// Flags
      lv_obj_set_style_radius(obj, 0, LV_STATE_DEFAULT);
      lv_obj_set_grid_cell(
        obj,
        LV_GRID_ALIGN_STRETCH, col, 1,
        LV_GRID_ALIGN_STRETCH, row, 1
      );

      btn = lv_btn_create(obj);
      lv_obj_set_size(btn, 40, 40);
      lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
      lv_obj_set_style_radius(btn, 0, LV_STATE_DEFAULT);
      lv_obj_add_event_cb(btn, grid_btns_cb[i], LV_EVENT_CLICKED, NULL);

      label = lv_label_create(btn);
      lv_label_set_text_fmt(label, "%d", i+1);
      lv_obj_center(label);

      grid_btns[i] = btn;
  }

  set_grid_btn_cb(0, start_program);
  set_grid_btn_cb(1, end_program);


  // events
  lv_obj_add_flag(time_container, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(time_container, time_msgbox_open, LV_EVENT_CLICKED, NULL);

  lv_obj_add_flag(temp_lb, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(temp_lb, temp_msgbox_open, LV_EVENT_CLICKED, NULL);
}

void main_screen_update(lv_timer_t * timer){
  handle_queue();
  lv_label_set_text_fmt(temp_lb, "%d", setTemperature);
  if (started){
    start_alpha_animation(&time_separator_anim_status, time_separator_lb, anim_alpha_cb, 1000, 200);
    time_update_lb(remainingMinutes, time_h_lb, time_m_lb);
  }
  else {
    stop_alpha_animation(&time_separator_anim_status, time_separator_lb, anim_alpha_cb);
    time_update_lb(setDurationMinutes, time_h_lb, time_m_lb);
  }
}

void handle_queue(){
  q_node_t data;
  while(q_dequeue(ui_queue, &data)){
    printf("[UI] Handling event code = %d\n", data.event);
    if (data.event == UI_OVEN_IDLE){
      started = false;
    }
    if (data.event == UI_OVEN_STARTED){
      started = true;
    }
    if (data.event == UI_UPDATE_REMAINING_MINUTES){
      remainingMinutes = *(int*)data.payload;
    }
    if (data.event == UI_UPDATE_TEMPERATURE){
      temperature = *(int*)data.payload;
    }
  }
}

void temp_msgbox_open(lv_event_t *e){

  static const char * btns[] = {"Okay", ""};
  lv_obj_t * mbox = lv_msgbox_create(NULL, NULL, NULL, btns, false);
  lv_obj_t * mbox_content = lv_msgbox_get_content(mbox);

  lv_obj_add_event_cb(mbox, temp_msgbox_btn_cb, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_set_style_width(mbox, 200, 0);
  lv_obj_center(mbox);

  lv_obj_set_flex_flow(mbox_content, LV_FLEX_FLOW_ROW);
  lv_obj_center(mbox_content);

  lv_obj_t *decrease_btn = lv_btn_create(mbox_content);
  lv_obj_t *decrease_btn_lb = lv_label_create(decrease_btn);
  lv_label_set_text_fmt(decrease_btn_lb, "-");

  temp_msgbox_lb = lv_label_create(mbox_content);
  lv_obj_set_size(temp_msgbox_lb, 100, 40);
  lv_obj_set_style_text_font(temp_msgbox_lb, &lv_font_montserrat_32, LV_PART_MAIN| LV_STATE_DEFAULT);
  lv_obj_set_style_text_align(temp_msgbox_lb, LV_TEXT_ALIGN_CENTER, 0);
  lv_label_set_text_fmt(temp_msgbox_lb, "%d", setTemperature);

  lv_obj_t *increase_btn = lv_btn_create(mbox_content);
  lv_obj_t *increase_btn_lb = lv_label_create(increase_btn);
  lv_label_set_text_fmt(increase_btn_lb, "+");

  lv_obj_center(decrease_btn);
  lv_obj_center(temp_msgbox_lb);
  lv_obj_center(increase_btn);

  // event handlers
  lv_obj_add_event_cb(decrease_btn, temp_msgbox_dec_cb, LV_EVENT_ALL, &temp_data);
  lv_obj_add_event_cb(increase_btn, temp_msgbox_inc_cb, LV_EVENT_ALL, &temp_data);
}

void temp_msgbox_btn_cb(lv_event_t *e){
  setTemperature = *temp_data.value;
  lv_label_set_text_fmt(temp_lb, "%d", setTemperature);
  lv_msgbox_close(lv_event_get_current_target(e));
}

void temp_msgbox_dec_cb(lv_event_t *e){
  if(counter_event_inspector(e)){
    counter_user_data * user_data = (counter_user_data *)lv_event_get_user_data(e);
    *user_data->value -= (*user_data->value <= TEMPERATURE_MIN_VALUE) ? 0 : 1;
    lv_label_set_text_fmt(temp_msgbox_lb, "%d", *user_data->value);
  }
}

void temp_msgbox_inc_cb(lv_event_t *e){
  if(counter_event_inspector(e)){
    counter_user_data * user_data = (counter_user_data *)lv_event_get_user_data(e);
    *user_data->value += (*user_data->value >= TEMPERATURE_MAX_VALUE) ? 0 : 1;
    lv_label_set_text_fmt(temp_msgbox_lb, "%d", *user_data->value);
  }
}

void time_update_lb(int minutes, lv_obj_t * hours_lb, lv_obj_t * minutes_lb){
  lv_label_set_text_fmt(hours_lb, "%d", minutes / 60);
  if ((minutes % 60) < 10) {
    lv_label_set_text_fmt(minutes_lb, "0%d", minutes % 60);
  }
  else {
    lv_label_set_text_fmt(minutes_lb, "%d", minutes % 60);
  }
}

void time_msgbox_open(lv_event_t *e){

  static const char * btns[] =  {"Okay", ""};
  lv_obj_t * mbox = lv_msgbox_create(NULL, NULL, NULL, btns, false);
  lv_obj_t * mbox_content = lv_msgbox_get_content(mbox);

  lv_obj_add_event_cb(mbox, time_msgbox_btn_cb, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_set_style_width(mbox, 200, 0);
  lv_obj_center(mbox);

  lv_obj_set_flex_flow(mbox_content, LV_FLEX_FLOW_ROW);
  lv_obj_center(mbox_content);

  lv_obj_t *decrease_btn = lv_btn_create(mbox_content);
  lv_obj_t *decrease_btn_lb = lv_label_create(decrease_btn);
  lv_label_set_text_fmt(decrease_btn_lb, "-");

  time_msgbox_h_lb = lv_label_create(mbox_content);
  lv_obj_set_size(time_msgbox_h_lb, 45, 40);
  lv_obj_set_style_text_font(time_msgbox_h_lb, &lv_font_montserrat_32, LV_PART_MAIN| LV_STATE_DEFAULT);
  lv_obj_set_style_text_align(time_msgbox_h_lb, LV_TEXT_ALIGN_RIGHT, 0);

  lv_obj_t *double_dot_lbl = lv_label_create(mbox_content);
  lv_label_set_text_fmt(double_dot_lbl, ":");
  lv_obj_set_size(double_dot_lbl, 10, 40);
  lv_obj_set_style_text_font(double_dot_lbl, &lv_font_montserrat_32, LV_PART_MAIN| LV_STATE_DEFAULT);

  time_msgbox_m_lb = lv_label_create(mbox_content);
  lv_obj_set_size(time_msgbox_m_lb, 45, 40);
  lv_obj_set_style_text_font(time_msgbox_m_lb, &lv_font_montserrat_32, LV_PART_MAIN| LV_STATE_DEFAULT);
  lv_obj_set_style_text_align(time_msgbox_m_lb, LV_TEXT_ALIGN_LEFT, 0);

  time_update_lb(*time_data.value, time_msgbox_h_lb, time_msgbox_m_lb);

  lv_obj_t *increase_btn = lv_btn_create(mbox_content);
  lv_obj_t *increase_btn_lb = lv_label_create(increase_btn);
  lv_label_set_text_fmt(increase_btn_lb, "+");

  lv_obj_center(decrease_btn);
  lv_obj_center(time_msgbox_h_lb);
  lv_obj_center(double_dot_lbl);
  lv_obj_center(time_msgbox_m_lb);
  lv_obj_center(increase_btn);

  // event handlers
  lv_obj_add_event_cb(decrease_btn, time_msgbox_inc_cb, LV_EVENT_ALL, &time_data);
  lv_obj_add_event_cb(increase_btn, time_msgbox_dec_cb, LV_EVENT_ALL, &time_data);

}

void time_msgbox_btn_cb(lv_event_t *e){
  setDurationMinutes = *time_data.value;
  time_update_lb(setDurationMinutes, time_h_lb, time_m_lb);
  lv_msgbox_close(lv_event_get_current_target(e));
}

void time_msgbox_inc_cb(lv_event_t *e){
  if(counter_event_inspector(e)){
    counter_user_data * user_data = (counter_user_data *)lv_event_get_user_data(e);
    *user_data->value -= (*user_data->value <= MINUTES_MIN_VALUE) ? 0 : 1;
    time_update_lb(*user_data->value, time_msgbox_h_lb, time_msgbox_m_lb);
  }
}

void time_msgbox_dec_cb(lv_event_t *e){
  if(counter_event_inspector(e)){
    counter_user_data * user_data = (counter_user_data *)lv_event_get_user_data(e);
    *user_data->value += (*user_data->value >= MINUTES_MAX_VALUE) ? 0 : 1;
    time_update_lb(*user_data->value, time_msgbox_h_lb, time_msgbox_m_lb);
  }
}



static void anim_alpha_cb(void * var, int32_t v){
  lv_obj_set_style_text_opa((lv_obj_t *)var, v, LV_STATE_DEFAULT);
}

void start_alpha_animation(lv_anim_enable_t * status, lv_obj_t * obj, lv_anim_exec_xcb_t cb, int32_t duration, int32_t delay){
  if (*status == LV_ANIM_ON){
    return;
  }

  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_var(&a, time_separator_lb);
  lv_anim_set_values(&a, 255, 0);
  lv_anim_set_time(&a, duration);
  lv_anim_set_playback_time(&a, duration);
  lv_anim_set_playback_delay(&a, delay);
  lv_anim_set_repeat_delay(&a, delay);
  lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
  lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);
  lv_anim_set_exec_cb(&a, cb);
  lv_anim_start(&a);
  *status = LV_ANIM_ON;
}

void stop_alpha_animation(lv_anim_enable_t * status, lv_obj_t * obj, lv_anim_exec_xcb_t cb){
  if (*status == LV_ANIM_OFF){
    return;
  }

  printf("Stopping animation\n");
  lv_anim_del(obj, cb);
  anim_alpha_cb(obj, 255);
  *status = LV_ANIM_OFF;
}
