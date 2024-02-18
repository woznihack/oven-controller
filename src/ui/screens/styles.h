#ifndef STYLES_H
#define STYLES_H

#include <lvgl.h>


/* styles */
lv_style_t container_style;
lv_style_t btns_style;
lv_style_t btns_checked_style;
lv_style_t btns_medium_style;
lv_style_t btns_circle_style;

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
  // lv_style_set_bg_color(&btns_circle_style, lv_color_hex(0xFF8000));
  // lv_style_set_bg_opa(&btns_circle_style, LV_OPA_100);
  lv_style_set_radius(&btns_circle_style, 40);
  lv_style_set_width(&btns_circle_style, 40);
  lv_style_set_height(&btns_circle_style, 40);
  lv_style_set_text_font(&btns_circle_style, &lv_font_montserrat_44);
}

#endif
