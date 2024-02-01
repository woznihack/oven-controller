#pragma once

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "queue.h"

extern q_queue_t * oven_queue;
extern q_queue_t * ui_queue;

typedef enum {
  OVEN_S_IDLE = 0,
  OVEN_S_PREHEAT,
  OVEN_S_MANTAIN,
  OVEN_S_COOLDOWN
} oven_state_t;

typedef enum {
  OVEN_S_LIGHT_ON = 0,
  OVEN_S_LIGHT_OFF
} oven_light_state_t;

typedef enum {
  OVEN_S_FAN_ON = 0,
  OVEN_S_FAN_OFF
} oven_fan_state_t;

typedef enum {
  OVEN_S_TOP_HEATER_ON = 0,
  OVEN_S_TOP_HEATER_OFF
} oven_top_heater_state_t;

typedef enum {
  OVEN_S_BOTTOM_HEATER_ON = 0,
  OVEN_S_BOTTOM_HEATER_OFF
} oven_bottom_heater_state_t;

typedef struct {
  oven_state_t state;
  void (*stateFunc)(void);
} oven_state_node_t;

typedef struct {
  oven_light_state_t state;
  void (*stateFunc)(void);
} oven_light_state_node_t;

typedef struct {
  oven_fan_state_t state;
  void (*stateFunc)(void);
} oven_fan_state_node_t;

typedef struct {
  oven_top_heater_state_t state;
  void (*stateFunc)(void);
} oven_top_heater_state_node_t;

typedef struct {
  oven_bottom_heater_state_t state;
  void (*stateFunc)(void);
} oven_bottom_heater_state_node_t;


void controller_init();
void controller_loop();

#endif // CONTROLLER_H
