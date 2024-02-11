#ifndef MESSAGES_H
#define MESSAGES_H

#include <lvgl.h>
#include "queue.h"

#define MSG_SET_CONF_DURATION_M        10
#define MSG_SET_CONF_TEMP              11

#define MSG_SET_MONITOR_REMAINING_M      10
#define MSG_SET_MONITOR_TOP_HEATER_TEMP  11
#define MSG_SET_MONITOR_DECK_HEATER_TEMP 12

#define MSG_SET_DURATION_M 20
#define MSG_SET_TEMPERATURE 21

#define MSG_SET_STATUS_STARTED 30
#define MSG_SET_STATUS_STOPPED 31
#define MSG_SET_STATUS_PAUSED  32
#define MSG_SET_STATUS_RESUMED 32

#define MSG_SET_BAKING_PHASE  40
#endif // MESSAGES_H
