
#include "toggles.h"
#include "styles.h"
#include <lvgl.h>

LV_IMG_DECLARE(icons_light_on)
LV_IMG_DECLARE(icons_light_off)
LV_IMG_DECLARE(icons_fan_on)
LV_IMG_DECLARE(icons_fan_off)
LV_IMG_DECLARE(icons_grill_on)
LV_IMG_DECLARE(icons_grill_off)
LV_IMG_DECLARE(icons_top_heater_on)
LV_IMG_DECLARE(icons_top_heater_off)
LV_IMG_DECLARE(icons_deck_heater_on)
LV_IMG_DECLARE(icons_deck_heater_off)

typedef struct {
  toggles_name_t name;
  const void *released;
  const void *checked_released;
} toggles_img_src_t;

static toggles_img_src_t toggles_img_src[] = {{TOGGLE_FAN, &icons_fan_off, &icons_fan_on}, {TOGGLE_LIGHT, &icons_light_off, &icons_light_on}, {TOGGLE_TOP_HEATER, &icons_top_heater_off, &icons_top_heater_on}, {TOGGLE_DECK_HEATER, &icons_deck_heater_off, &icons_deck_heater_on}, {TOGGLE_GRILL, &icons_grill_off, &icons_grill_on}};

lv_obj_t *toggle_create(lv_obj_t *parent, toggles_name_t name) {
  LV_IMG_DECLARE(icons_light_on)
  LV_IMG_DECLARE(icons_light_off)
  lv_obj_t *btn = lv_imgbtn_create(parent);
  lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
  lv_obj_add_style(btn, &styles->transparent_btn, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style(btn, &styles->squared_btn, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_imgbtn_set_src(btn, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, toggles_img_src[name].checked_released, NULL);
  lv_imgbtn_set_src(btn, LV_IMGBTN_STATE_RELEASED, NULL, toggles_img_src[name].released, NULL);
  return btn;
}
