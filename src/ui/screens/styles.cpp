#include "styles.h"

#include <lvgl.h>

/* styles */
lv_style_t main_container_style;
lv_style_t container_style;
lv_style_t counter_btn_style;
lv_style_t counter_btn_pressed_style;
lv_style_t counter_btn_disabled_style;
lv_style_t counter_btn_img_style;
lv_style_t toggle_btn_style;

void init_styles() {
  lv_style_init(&main_container_style);
  lv_style_set_bg_color(&main_container_style, lv_color_hex(0x313131));
  lv_style_set_flex_flow(&main_container_style, LV_FLEX_FLOW_COLUMN);
  lv_style_set_pad_all(&main_container_style, 0);
  lv_style_set_pad_row(&main_container_style, 0);

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

  lv_style_init(&counter_btn_style);
  lv_style_set_bg_opa(&counter_btn_style, LV_OPA_0);
  lv_style_set_prop_meta(&counter_btn_style, LV_STYLE_IMG_OPA, LV_STYLE_PROP_META_INHERIT);
  lv_style_set_prop_meta(&counter_btn_style, LV_STYLE_IMG_RECOLOR, LV_STYLE_PROP_META_INHERIT);
  lv_style_set_prop_meta(&counter_btn_style, LV_STYLE_IMG_RECOLOR_OPA, LV_STYLE_PROP_META_INHERIT);
  lv_style_set_border_width(&counter_btn_style, 0);
  lv_style_set_outline_width(&counter_btn_style, 0);
  lv_style_set_shadow_width(&counter_btn_style, 0);
  lv_style_set_img_recolor(&counter_btn_style, lv_color_hex(0xcccccc));
  lv_style_set_img_recolor_opa(&counter_btn_style, LV_OPA_100);

  lv_style_init(&counter_btn_pressed_style);
  lv_style_set_bg_color(&counter_btn_pressed_style, lv_color_hex(0xe6e6e6ff));
  lv_style_set_bg_opa(&counter_btn_pressed_style, LV_OPA_70);

  lv_style_init(&counter_btn_disabled_style);

  lv_style_init(&counter_btn_img_style);

  lv_style_init(&toggle_btn_style);
  lv_style_set_bg_color(&toggle_btn_style, lv_color_hex(0xFF8000));
  lv_style_set_bg_opa(&toggle_btn_style, LV_OPA_100);
  lv_style_set_size(&toggle_btn_style, 40);
  lv_style_set_bg_opa(&toggle_btn_style, LV_OPA_0);
}
