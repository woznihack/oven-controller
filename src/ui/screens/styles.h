#ifndef STYLES_H
#define STYLES_H

#include <lvgl.h>

/* styles */
extern lv_style_t main_container_style;
extern lv_style_t container_style;
extern lv_style_t counter_btn_style;
extern lv_style_t counter_btn_pressed_style;
extern lv_style_t counter_btn_disabled_style;
extern lv_style_t counter_btn_img_style;
extern lv_style_t toggle_btn_style;

void init_styles();

#endif
