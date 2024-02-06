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

typedef enum
{
  OVEN_S_TOP_HEATER_ON = 0,
  OVEN_S_TOP_HEATER_OFF
} oven_top_heater_state_t;

typedef enum
{
  OVEN_S_DECK_HEATER_ON = 0,
  OVEN_S_DECK_HEATER_OFF
} oven_deck_heater_state_t;

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

typedef struct
{
  oven_top_heater_state_t state;
  void (*stateFunc)(void);
} oven_top_heater_state_node_t;

typedef struct
{
  oven_deck_heater_state_t state;
  void (*stateFunc)(void);
} oven_deck_heater_state_node_t;

// controller
void controller_init();
void controller_loop();

// light state fn
void light_on();
void light_off();

// fan state fn
void fan_on();
void fan_off();

// fan heater fn
void top_heater_on();
void top_heater_off();
void deck_heater_on();
void deck_heater_off();

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

static oven_top_heater_state_node_t top_heater_sm[] = {
    // light state machine
    {OVEN_S_TOP_HEATER_ON, top_heater_on},
    {OVEN_S_TOP_HEATER_OFF, top_heater_off},
};

static oven_deck_heater_state_node_t deck_heater_sm[] = {
    // light state machine
    {OVEN_S_DECK_HEATER_ON, deck_heater_on},
    {OVEN_S_DECK_HEATER_OFF, deck_heater_off},
};

extern oven_light_state_t current_light_state;
extern oven_fan_state_t current_fan_state;
extern oven_top_heater_state_t current_top_heater_state;
extern oven_deck_heater_state_t current_deck_heater_state;

extern uint32_t top_heater_temperature;
extern uint32_t deck_heater_temperature;
extern uint32_t baking_duration_m;
extern uint32_t baking_temperature;
extern uint32_t baking_remaining_m;

#endif // CONTROLLER_H
