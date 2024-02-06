#include <stdio.h>
#include <lvgl.h>

#include "ui.h"
#include "counter.h"
#include "animations.h"
#include "helpers.h"
#include "messages.h"
#include "screens/screens.h"

// styles
static lv_style_t main_style;
static lv_style_t alpha_bg_style;

struct Status
{
  bool started;
  uint32_t remaining_m;
  uint32_t top_heater_temp;
  uint32_t bot_heater_temp;
};

static struct Status status = {
    .started = true,
    .remaining_m = 10,
    .top_heater_temp = 10,
    .bot_heater_temp = 10};

struct Settings settings;

void handle_queue(lv_timer_t *);

void ui_msg_handler(void *s, lv_msg_t *msg)
{
  uint32_t id = lv_msg_get_id(msg);
  switch (id)
  {
  case MSG_SET_STATUS_STARTED:
    if (baking_scr == NULL)
    {
      baking_scr_init();
    }
    lv_scr_load_anim(baking_scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 300, false);

    q_enqueue(oven_queue, OVEN_SET_TEMPERATURE, &settings.temperature.value);
    q_enqueue(oven_queue, OVEN_SET_DURATION_M, &settings.duration_m.value);
    q_enqueue(oven_queue, OVEN_START, NULL);
    break;
  case MSG_SET_STATUS_STOPPED:
    lv_scr_load_anim(program_setup_scr, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 300, false);
    q_enqueue(oven_queue, OVEN_STOP, NULL);
    break;
  default:
  }
}

void ui_init(void)
{
  settings.program_name = "Manual";
  settings.temperature = counter_create(TEMPERATURE_MIN_VALUE);
  settings.duration_m = counter_create(DURATION_MIN_VALUE);

  printf("-- UI SETUP --");
  printf("Setting.temperature = %d\n", settings.temperature.value);
  printf("Setting.duration_m = %d\n", settings.duration_m.value);

  splash_scr_init();
  program_setup_scr_init();

  lv_disp_load_scr(splash_scr);
  lv_scr_load_anim(program_setup_scr, LV_SCR_LOAD_ANIM_FADE_ON, 1000, 1000, true);
  lv_timer_t *main_screen_update_timer = lv_timer_create(handle_queue, 100, NULL);

  lv_msg_subscribe(MSG_SET_STATUS_STARTED, ui_msg_handler, NULL);
  lv_msg_subscribe(MSG_SET_STATUS_STOPPED, ui_msg_handler, NULL);
}

void handle_queue(lv_timer_t *timer)
{
  q_node_t data;
  while (q_dequeue(ui_queue, &data))
  {
    switch (data.event)
    {
    case UI_UPDATE_BAKING_PHASE:
    {
      lv_msg_send(MSG_SET_BAKING_PHASE, data.payload);
      break;
    }
    case UI_UPDATE_REMAINING_M:
    {
      lv_msg_send(MSG_SET_MONITOR_REMAINING_M, data.payload);
      break;
    }
    case UI_UPDATE_TOP_HEATER_TEMP:
    {

      lv_msg_send(MSG_SET_MONITOR_TOP_HEATER_TEMP, data.payload);
      break;
    }

    case UI_UPDATE_DECK_HEATER_TEMP:
    {
      lv_msg_send(MSG_SET_MONITOR_DECK_HEATER_TEMP, data.payload);
      break;
    }
    default:
    }
  }
}

// void set_grid_btn_cb(int idx, lv_event_cb_t cb)
// {
//   lv_obj_remove_event_cb(grid_btns[idx], grid_btns_cb[idx]);
//   grid_btns_cb[idx] = cb;
//   lv_obj_add_event_cb(grid_btns[idx], grid_btns_cb[idx], LV_EVENT_CLICKED, NULL);
// }

// void styles_init()
// {
//   lv_style_init(&main_style);
//   lv_style_set_border_width(&main_style, 0);
//   lv_style_set_outline_width(&main_style, 1);
//   lv_style_set_pad_all(&main_style, 0);
//   lv_style_set_pad_row(&main_style, 0);
//   lv_style_set_pad_column(&main_style, 0);
//   lv_style_set_radius(&main_style, 0);

//   lv_style_init(&alpha_bg_style);
//   lv_style_set_bg_opa(&alpha_bg_style, LV_OPA_0);
// }

// void draw_items()
// {
//   main_screen = lv_obj_create(NULL);
//   lv_obj_add_style(main_screen, &main_style, 0);
//   lv_obj_set_scrollbar_mode(main_screen, LV_SCROLLBAR_MODE_OFF);
//   lv_obj_clear_flag(main_screen, LV_OBJ_FLAG_SCROLLABLE);

//   lv_obj_t *main_panel = lv_obj_create(main_screen);
//   lv_obj_set_scrollbar_mode(main_panel, LV_SCROLLBAR_MODE_OFF);
//   lv_obj_clear_flag(main_panel, LV_OBJ_FLAG_SCROLLABLE);
//   lv_obj_set_size(main_panel, 240, 320);
//   lv_obj_align(main_panel, LV_ALIGN_TOP_LEFT, 0, 0);
//   lv_obj_add_style(main_panel, &main_style, 0);

//   static lv_coord_t main_col_dsc[] = {240, LV_GRID_TEMPLATE_LAST};
//   static lv_coord_t main_row_dsc[] = {100, 100, 120, LV_GRID_TEMPLATE_LAST};
//   lv_obj_set_grid_dsc_array(main_panel, main_col_dsc, main_row_dsc);

//   top_panel = lv_obj_create(main_panel);
//   middle_panel = lv_obj_create(main_panel);
//   bot_panel = lv_obj_create(main_panel);

//   lv_obj_set_grid_cell(top_panel, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
//   lv_obj_set_grid_cell(middle_panel, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
//   lv_obj_set_grid_cell(bot_panel, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 2, 1);

//   lv_obj_add_style(top_panel, &main_style, 0);
//   lv_obj_add_style(middle_panel, &main_style, 0);
//   lv_obj_add_style(bot_panel, &main_style, 0);

//   /* top panel content */
//   lv_obj_t *program_name = lv_label_create(top_panel);
//   lv_obj_add_style(program_name, &main_style, 0);
//   lv_obj_add_style(program_name, &alpha_bg_style, 0);
//   lv_label_set_text_fmt(program_name, "%s", settings.program_name);

//   time_container = lv_obj_create(top_panel);
//   lv_obj_center(time_container);
//   lv_obj_add_style(time_container, &main_style, 0);
//   lv_obj_add_style(time_container, &alpha_bg_style, 0);
//   lv_obj_clear_flag(time_container, LV_OBJ_FLAG_SCROLLABLE);
//   lv_obj_set_size(time_container, 70, 32);
//   lv_obj_set_style_outline_width(time_container, 1, LV_STATE_DEFAULT);
//   lv_obj_add_flag(time_container, LV_OBJ_FLAG_CLICKABLE);
//   lv_msg_subscribe_obj(MSG_SET_CONF_DURATION_M, time_container, NULL);
//   lv_obj_add_event_cb(time_container, duration_container_event_cb, LV_EVENT_CLICKED, NULL);
//   lv_obj_add_event_cb(time_container, duration_container_event_cb, LV_EVENT_MSG_RECEIVED, NULL);

//   time_h_lb = lv_label_create(time_container);
//   lv_obj_set_style_text_font(time_h_lb, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
//   lv_obj_set_style_text_align(time_h_lb, LV_TEXT_ALIGN_RIGHT, 0);

//   time_separator_lb = lv_label_create(time_container);
//   lv_label_set_text_fmt(time_separator_lb, ":");
//   lv_obj_set_style_text_font(time_separator_lb, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);

//   time_m_lb = lv_label_create(time_container);
//   lv_obj_set_style_text_font(time_m_lb, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
//   lv_obj_set_style_text_align(time_h_lb, LV_TEXT_ALIGN_LEFT, 0);

//   time_update_lb(settings.duration_m, time_h_lb, time_m_lb);

//   lv_obj_align(time_m_lb, LV_ALIGN_RIGHT_MID, 0, 0);
//   lv_obj_align_to(time_separator_lb, time_m_lb, LV_ALIGN_OUT_LEFT_MID, 0, 0);
//   lv_obj_align_to(time_h_lb, time_separator_lb, LV_ALIGN_OUT_LEFT_MID, 0, 0);

//   /* middle panel content */
//   temp_lb = lv_label_create(middle_panel);
//   lv_obj_center(temp_lb);
//   lv_label_set_text_fmt(temp_lb, "%d °C", settings.temperature);
//   lv_obj_set_style_text_font(temp_lb, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);
//   lv_obj_add_flag(temp_lb, LV_OBJ_FLAG_CLICKABLE);
//   lv_msg_subscribe_obj(MSG_SET_CONF_TEMP, temp_lb, NULL);
//   lv_obj_add_event_cb(temp_lb, temperature_container_event_cb, LV_EVENT_CLICKED, NULL);
//   lv_obj_add_event_cb(temp_lb, temperature_container_event_cb, LV_EVENT_MSG_RECEIVED, NULL);

//   /* bottom panel content */
//   static lv_coord_t col_dsc[] = {60, 60, 60, 60, LV_GRID_TEMPLATE_LAST};
//   static lv_coord_t row_dsc[] = {60, 60, LV_GRID_TEMPLATE_LAST};
//   lv_obj_set_grid_dsc_array(bot_panel, col_dsc, row_dsc);
//   lv_obj_set_scrollbar_mode(bot_panel, LV_SCROLLBAR_MODE_OFF);

//   lv_obj_t *label;
//   lv_obj_t *obj;
//   lv_obj_t *btn;
//   uint32_t i;
//   for (i = 0; i < 8; i++)
//   {
//     uint8_t col = i % 4;
//     uint8_t row = i / 4;

//     obj = lv_obj_create(bot_panel);
//     lv_obj_add_style(obj, &main_style, 0);
//     lv_obj_add_style(obj, &alpha_bg_style, 0);
//     lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE); /// Flags
//     lv_obj_set_style_radius(obj, 0, LV_STATE_DEFAULT);
//     lv_obj_set_grid_cell(
//         obj,
//         LV_GRID_ALIGN_STRETCH, col, 1,
//         LV_GRID_ALIGN_STRETCH, row, 1);

//     btn = lv_btn_create(obj);
//     lv_obj_set_size(btn, 40, 40);
//     lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
//     lv_obj_set_style_radius(btn, 0, LV_STATE_DEFAULT);
//     lv_obj_add_event_cb(btn, grid_btns_cb[i], LV_EVENT_CLICKED, NULL);

//     label = lv_label_create(btn);
//     lv_label_set_text_fmt(label, "%d", i + 1);
//     lv_obj_center(label);

//     grid_btns[i] = btn;
//   }
// }

// void duration_container_event_cb(lv_event_t *e)
// {

//   printf("duration_container_event_cb called...");
//   lv_event_code_t code = lv_event_get_code(e);
//   if (code == LV_EVENT_CLICKED){

//     printf(".. for a click!\n");
//     // duration_mbox_open(settings.duration_m);
//   }
//   else if (code == LV_EVENT_MSG_RECEIVED){
//     printf(".. for a message!\n");
//     lv_msg_t * m = lv_event_get_msg(e);
//     // lv_obj_t * target = lv_event_get_target(e);
//     if (lv_msg_get_id(m) == MSG_SET_CONF_DURATION_M){
//       settings.duration_m = *(uint32_t *)lv_msg_get_payload(m);
//       time_update_lb(settings.duration_m, time_h_lb, time_m_lb);
//     }
//   }
//   else {
//     printf("\n");
//   }
// }

// void temperature_container_event_cb(lv_event_t *e)
// {

//   printf("temperature_container_event_cb called...");
//   lv_event_code_t code = lv_event_get_code(e);
//   if (code == LV_EVENT_CLICKED){

//     printf(".. for a click!\n");
//     // temperature_mbox_open(settings.temperature);
//   }
//   else if (code == LV_EVENT_MSG_RECEIVED){
//     printf(".. for a message!\n");
//     lv_msg_t * m = lv_event_get_msg(e);
//     // lv_obj_t * target = lv_event_get_target(e);
//     if (lv_msg_get_id(m) == MSG_SET_CONF_TEMP){
//       settings.temperature = *(uint32_t *)lv_msg_get_payload(m);
//       lv_label_set_text_fmt(temp_lb, "%d °C", settings.temperature);
//     }
//   }
//   else {
//     printf("\n");
//   }
// }
