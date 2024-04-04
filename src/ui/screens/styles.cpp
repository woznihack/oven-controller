#include "styles.h"

#include <lvgl.h>

styles_t* styles;
static lv_color_t grey_filter_cb(const lv_color_filter_dsc_t* f, lv_color_t color, lv_opa_t opa) {
  LV_UNUSED(f);
  return lv_color_mix(lv_palette_darken(LV_PALETTE_GREY, 2), color, opa);
}
void init_styles() {
  if (styles == NULL) {
    styles = (styles_t*)malloc(sizeof(styles_t));
  }

  lv_style_init(&styles->top_bar);
  lv_style_set_pad_all(&styles->top_bar, 0);
  lv_style_set_pad_row(&styles->top_bar, 0);
  lv_style_set_align(&styles->top_bar, LV_ALIGN_CENTER);
  lv_style_set_width(&styles->top_bar, LV_PCT(100));
  lv_style_set_bg_opa(&styles->top_bar, LV_OPA_0);
  lv_style_set_radius(&styles->top_bar, 0);
  lv_style_set_outline_width(&styles->top_bar, 0);
  lv_style_set_border_width(&styles->top_bar, 0);
  lv_style_set_border_opa(&styles->top_bar, LV_OPA_30);
  lv_style_set_text_color(&styles->top_bar, lv_color_white());

  lv_style_init(&styles->flex_col_container);
  lv_style_set_bg_color(&styles->flex_col_container, lv_color_hex(0x313131));
  lv_style_set_layout(&styles->flex_col_container, LV_LAYOUT_FLEX);
  lv_style_set_flex_flow(&styles->flex_col_container, LV_FLEX_FLOW_COLUMN);
  lv_style_set_pad_all(&styles->flex_col_container, 0);
  lv_style_set_pad_row(&styles->flex_col_container, 0);
  lv_style_set_align(&styles->flex_col_container, LV_ALIGN_CENTER);

  lv_style_init(&styles->flex_row_container);
  lv_style_set_layout(&styles->flex_row_container, LV_LAYOUT_FLEX);
  lv_style_set_flex_flow(&styles->flex_row_container, LV_FLEX_FLOW_ROW);
  lv_style_set_width(&styles->flex_row_container, LV_PCT(100));
  lv_style_set_bg_opa(&styles->flex_row_container, LV_OPA_0);
  lv_style_set_radius(&styles->flex_row_container, 0);
  lv_style_set_outline_width(&styles->flex_row_container, 0);
  lv_style_set_border_width(&styles->flex_row_container, 0);
  lv_style_set_border_opa(&styles->flex_row_container, LV_OPA_30);
  lv_style_set_pad_all(&styles->flex_row_container, 0);
  lv_style_set_pad_column(&styles->flex_row_container, 2);
  lv_style_set_text_color(&styles->flex_row_container, lv_color_white());
  lv_style_set_align(&styles->flex_row_container, LV_ALIGN_CENTER);
  lv_style_set_text_align(&styles->flex_row_container, LV_TEXT_ALIGN_CENTER);

  lv_style_init(&styles->flex_row_container_title);
  lv_style_set_width(&styles->flex_row_container_title, LV_PCT(100));
  lv_style_set_text_font(&styles->flex_row_container_title, &lv_font_montserrat_10);
  lv_style_set_text_align(&styles->flex_row_container_title, LV_TEXT_ALIGN_CENTER);
  lv_style_set_y(&styles->flex_row_container_title, 10);

  lv_style_init(&styles->centered_content_container);
  lv_style_set_bg_opa(&styles->centered_content_container, LV_OPA_0);
  lv_style_set_align(&styles->centered_content_container, LV_ALIGN_CENTER);
  lv_style_set_text_align(&styles->centered_content_container, LV_TEXT_ALIGN_CENTER);
  lv_style_set_outline_width(&styles->centered_content_container, 0);
  lv_style_set_border_width(&styles->centered_content_container, 0);
  lv_style_set_text_color(&styles->centered_content_container, lv_color_white());

  lv_style_init(&styles->transparent_btn);
  lv_style_set_radius(&styles->transparent_btn, 0);
  lv_style_set_bg_opa(&styles->transparent_btn, LV_OPA_0);
  lv_style_set_border_width(&styles->transparent_btn, 0);
  lv_style_set_outline_width(&styles->transparent_btn, 0);
  lv_style_set_shadow_width(&styles->transparent_btn, 0);
  lv_style_set_height(&styles->transparent_btn, LV_PCT(100));

  lv_style_init(&styles->transparent_pressed_btn);
  lv_style_set_bg_color(&styles->transparent_pressed_btn, lv_color_hex(0xe6e6e6));
  lv_style_set_bg_opa(&styles->transparent_pressed_btn, LV_OPA_70);
  lv_style_set_pad_all(&styles->transparent_pressed_btn, 0);

  lv_style_init(&styles->steps_bar_btn);
  lv_style_set_flex_grow(&styles->steps_bar_btn, 1);
  lv_style_set_pad_all(&styles->steps_bar_btn, 0);
  lv_style_set_height(&styles->steps_bar_btn, LV_PCT(100));
  lv_style_set_text_font(&styles->steps_bar_btn, &lv_font_montserrat_14);
  lv_style_set_border_width(&styles->steps_bar_btn, 2);
  lv_style_set_border_color(&styles->steps_bar_btn, lv_color_hex(0xffffff));
  lv_style_set_border_side(&styles->steps_bar_btn, LV_BORDER_SIDE_BOTTOM);

  lv_style_init(&styles->steps_bar_active_btn);
  lv_style_set_text_color(&styles->steps_bar_active_btn, lv_color_hex(0xffb43c));
  lv_style_set_border_color(&styles->steps_bar_active_btn, lv_color_hex(0xffb43c));

  lv_style_init(&styles->btns_bar_btn);
  lv_style_set_flex_grow(&styles->btns_bar_btn, 1);
  lv_style_set_pad_all(&styles->btns_bar_btn, 0);
  lv_style_set_height(&styles->btns_bar_btn, LV_PCT(100));
  lv_style_set_text_font(&styles->btns_bar_btn, &lv_font_montserrat_12);
  lv_style_set_border_width(&styles->btns_bar_btn, 1);
  lv_style_set_border_color(&styles->btns_bar_btn, lv_color_hex(0xffb43c));
  lv_style_set_text_color(&styles->btns_bar_btn, lv_color_hex(0xffb43c));

  lv_style_init(&styles->btns_bar_main_btn);
  lv_style_set_bg_color(&styles->btns_bar_main_btn, lv_color_hex(0xffb43c));
  lv_style_set_bg_opa(&styles->btns_bar_main_btn, LV_OPA_90);
  lv_style_set_text_color(&styles->btns_bar_main_btn, lv_color_hex(0xffffff));

  static lv_color_filter_dsc_t grey_filter;
  lv_color_filter_dsc_init(&grey_filter, grey_filter_cb);

  lv_style_init(&styles->disabled);
  lv_style_set_color_filter_dsc(&styles->disabled, &grey_filter);
  lv_style_set_color_filter_opa(&styles->disabled, LV_OPA_50);

  lv_style_init(&styles->squared_btn);
  lv_style_set_size(&styles->squared_btn, 40);
}
