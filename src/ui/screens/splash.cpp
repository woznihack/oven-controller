#include <stdio.h>
#include <lvgl.h>

#include "screens.h"

lv_obj_t *splash_scr;

void splash_scr_init()
{
  splash_scr = lv_obj_create(NULL);
  lv_obj_clear_flag(splash_scr, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_obj_set_style_bg_color(splash_scr, lv_color_hex(0x3E3E3E), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(splash_scr, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  LV_IMG_DECLARE(logo)
  lv_obj_t *logo_img = lv_img_create(splash_scr);
  lv_img_set_src(logo_img, &logo);
  lv_obj_align(logo_img, LV_ALIGN_CENTER, 0, -20);
}
