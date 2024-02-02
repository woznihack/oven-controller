#ifndef DURATION_MBOX_H
#define DURATION_MBOX_H

#include "lvgl.h"

#define DURATION_MAX_VALUE 600
#define DURATION_MIN_VALUE 1

void duration_mbox_open_cb(lv_event_t *);
void duration_mbox_open();

#endif
