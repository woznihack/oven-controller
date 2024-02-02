#ifndef MESSAGES_H
#define MESSAGES_H

#include <lvgl.h>
#include "queue.h"

#define MSG_SET_CONF_DURATION_M        10
#define MSG_SET_CONF_TEMP              11

#define MSG_SET_STATUS_REMAINING_M     20
#define MSG_SET_STATUS_TOP_HEATER_TEMP 21
#define MSG_SET_STATUS_BOT_HEATER_TEMP 22
#define MSG_SET_STATUS_STARTED         23
#define MSG_SET_STATUS_STOPPED         24

#endif // MESSAGES_H
