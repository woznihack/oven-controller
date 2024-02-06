#ifndef UI_H
#define UI_H

#include <lvgl.h>
#include "../queue.h"
#include "../settings.h"
#include "counter.h"

extern const lv_img_dsc_t logo;

extern q_queue_t * oven_queue;
extern q_queue_t * ui_queue;

void ui_init(void);

struct Settings
{
  char *program_name;
  counter_t temperature;
  counter_t duration_m;
};

extern struct Settings settings;

#endif // UI_H
