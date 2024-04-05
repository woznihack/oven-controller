#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <lvgl.h>

lv_obj_t *top_bar_create(lv_obj_t *parent, char *title, bool show_time, bool back_btn, lv_event_cb_t back_btn_event_cb);

#endif
