#ifndef MAIN_SCREEN_H
#define MAIN_SCREEN_H

#include <lvgl.h>

void main_screen_init();
void main_screen_update(lv_timer_t * timer);
void set_grid_btn_cb(int idx, lv_event_cb_t cb);

#endif // MAIN_SCREEN_H
