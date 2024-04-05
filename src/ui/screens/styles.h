#ifndef STYLES_H
#define STYLES_H

#include <lvgl.h>

/* styles */
typedef struct {
  lv_style_t top_bar;
  lv_style_t flex_col_container;
  lv_style_t flex_row_container;
  lv_style_t flex_row_container_title;
  lv_style_t transparent_btn;
  lv_style_t transparent_pressed_btn;
  lv_style_t squared_btn;
  lv_style_t steps_bar_btn;
  lv_style_t steps_bar_active_btn;
  lv_style_t btns_bar_btn;
  lv_style_t btns_bar_main_btn;
  lv_style_t disabled;
  lv_style_t centered_content_container;
} styles_t;

extern styles_t * styles;

void init_styles();

#endif
