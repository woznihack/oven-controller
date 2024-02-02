#ifndef MESSAGES_H
#define MESSAGES_H

#include <lvgl.h>
#include "queue.h"

#define MSG_DURATON_MINUTES_T_CHANGED  10
#define MSG_TEMPERATURE_T_CHANGED      11
#define MSG_REMAINING_MINUTES_CHANGED  12
#define MSG_TEMPERATURE_CHANGED        13

#define MSG_OVEN_STARTED               20
#define MSG_OVEN_STOPPED               21
#define MSG_PROGRAM_PAUSED             21

#endif // MESSAGES_H
