#ifndef UI_H
#define UI_H

#include <lvgl.h>

extern lv_obj_t *main_screen;
extern lv_obj_t *main_screen_button;
extern const lv_img_dsc_t logo;

void ui_init(void);
void set_grid_btn_cb(int idx, lv_event_cb_t cb);

#endif // UI_H
