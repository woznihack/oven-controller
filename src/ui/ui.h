#pragma once

#ifndef UI_H
#define UI_H

#include <lvgl.h>
#include "../queue.h"

extern const lv_img_dsc_t logo;
extern q_queue_t * oven_control_q;
extern q_queue_t * oven_monitor_q;

void ui_init(void);

#endif // UI_H
