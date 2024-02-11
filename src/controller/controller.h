#pragma once

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../queue.h"
#include "../settings.h"

extern q_queue_t *oven_queue;
extern q_queue_t *ui_queue;

typedef enum
{
  OVEN_S_LIGHT_ON = 0,
  OVEN_S_LIGHT_OFF
} oven_light_state_t;

typedef enum
{
  OVEN_S_FAN_ON = 0,
  OVEN_S_FAN_OFF
} oven_fan_state_t;

typedef struct
{
  oven_light_state_t state;
  void (*stateFunc)(void);
} oven_light_state_node_t;

typedef struct
{
  oven_fan_state_t state;
  void (*stateFunc)(void);
} oven_fan_state_node_t;

// controller
void controller_init();
void controller_loop();

// light state fn
void light_on();
void light_off();

// fan state fn
void fan_on();
void fan_off();

static oven_light_state_node_t light_sm[] = {
    // light state machine
    {OVEN_S_LIGHT_ON, light_on},
    {OVEN_S_LIGHT_OFF, light_off},
};

static oven_fan_state_node_t fan_sm[] = {
    // fan state machine
    {OVEN_S_FAN_ON, fan_on},
    {OVEN_S_FAN_OFF, fan_off},
};

// for debugging
const char *light_state_strings[] = {"ON", "OFF"};
const char *fan_state_strings[] = {"ON", "OFF"};
const char *top_heater_state_strings[] = {"ON", "OFF"};
const char *deck_heater_state_string[] = {"ON", "OFF"};

#endif // CONTROLLER_H
