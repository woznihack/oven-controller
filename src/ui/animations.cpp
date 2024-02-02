#include "lvgl.h"
#include "animations.h"

void anim_alpha_cb(void *var, int32_t v)
{
  lv_obj_set_style_text_opa((lv_obj_t *)var, v, LV_STATE_DEFAULT);
}

void start_alpha_animation(lv_anim_enable_t *status, lv_obj_t *obj, lv_anim_exec_xcb_t cb, int32_t duration, int32_t delay)
{
  if (*status == LV_ANIM_ON)
  {
    return;
  }

  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_var(&a, obj);
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

void stop_alpha_animation(lv_anim_enable_t *status, lv_obj_t *obj, lv_anim_exec_xcb_t cb)
{
  if (*status == LV_ANIM_OFF)
  {
    return;
  }

  lv_anim_del(obj, cb);
  anim_alpha_cb(obj, 255);
  *status = LV_ANIM_OFF;
}
