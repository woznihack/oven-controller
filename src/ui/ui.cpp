#include "ui.h"
#include "main-screen.h"

#include <stdio.h>
#include <lvgl.h>



void ui_init(void){

  lv_obj_t * init_screen = lv_obj_create(NULL);
  lv_obj_clear_flag( init_screen, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
  lv_obj_set_style_bg_color(init_screen, lv_color_hex(0x3E3E3E), LV_PART_MAIN | LV_STATE_DEFAULT );
  lv_obj_set_style_bg_opa(init_screen, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

  LV_IMG_DECLARE(logo)
  lv_obj_t * logo_img = lv_img_create(init_screen);
  lv_img_set_src(logo_img, &logo);
  lv_obj_align(logo_img, LV_ALIGN_CENTER, 0, -20);

  lv_disp_load_scr(init_screen);

  main_screen_init();
  lv_scr_load_anim(main_screen, LV_SCR_LOAD_ANIM_FADE_ON, 1000, 1000, true);
  lv_timer_t *main_screen_update_timer = lv_timer_create(main_screen_update, 100, NULL);

}
