#ifndef TOGGLES_H
#define TOGGLES_H

#include <lvgl.h>

#include "../../main.h"


typedef enum { TOGGLE_FAN, TOGGLE_LIGHT, TOGGLE_TOP_HEATER, TOGGLE_DECK_HEATER } toggles_name_t;

lv_obj_t *toggle_create(lv_obj_t *parent, toggles_name_t name);

#endif
