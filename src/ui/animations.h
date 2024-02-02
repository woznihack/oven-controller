#ifndef ANIMATIONS_H
#define ANIMATIONS_H


#include "lvgl.h"

void anim_alpha_cb(void *var, int32_t v);
void start_alpha_animation(lv_anim_enable_t *status, lv_obj_t *obj, lv_anim_exec_xcb_t cb, int32_t duration, int32_t delay);
void stop_alpha_animation(lv_anim_enable_t *status, lv_obj_t *obj, lv_anim_exec_xcb_t cb);

#endif
