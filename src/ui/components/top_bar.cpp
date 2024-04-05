#include "../../main.h"
#include "../screens/styles.h"
#include "components.h"

lv_obj_t *top_bar_create(lv_obj_t *parent, char *title, bool show_time, bool show_back_btn, lv_event_cb_t back_btn_event_cb) {
  lv_obj_t *tb = lv_obj_create(parent);
  lv_obj_set_scrollbar_mode(tb, LV_SCROLLBAR_MODE_OFF);
  lv_obj_add_style(tb, &styles->top_bar, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_size(tb, LV_PCT(100), 30);

  if (show_time) {
    lv_obj_t *time_lb = lv_label_create(tb);
    lv_obj_align(time_lb, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(time_lb, "10:20");
  }

  if (show_back_btn) {
    lv_obj_t *back_btn = lv_btn_create(tb);
    lv_obj_set_align(back_btn, LV_ALIGN_LEFT_MID);
    lv_obj_add_style(back_btn, &styles->transparent_btn, LV_STATE_DEFAULT);
    lv_obj_t *back_btn_lb = lv_label_create(back_btn);
    lv_label_set_text(back_btn_lb, "<");
    lv_obj_add_event_cb(back_btn, back_btn_event_cb, LV_EVENT_CLICKED, NULL);
  }

  return tb;
}
