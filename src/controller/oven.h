#pragma once

#ifndef OVEN_STATE_H
#define OVEN_STATE_H

#include <stdint.h>
#include "heater.h"

typedef enum
{
  OVEN_S_IDLE = 0,
  OVEN_S_PREHEATING,
  OVEN_S_BAKING,
  OVEN_S_COOLDOWN
} oven_state_t;

static const char *oven_state_strings[] = {
    "IDLE",
    "PREHEATING",
    "BAKING",
    "COOLDOWN"};

typedef struct
{
  oven_state_t current_state;
  heater_t top_heater;
  heater_t deck_heater;
  time_t state_start_s[4];
} oven_t;
typedef struct
{
  oven_state_t state;
  void (*stateFunc)(oven_t *);
  void (*initStateFunc)(oven_t *);
} oven_state_node_t;


// oven state init functions
void oven_idle_init(oven_t *);
void oven_preheating_init(oven_t *);
void oven_baking_init(oven_t *);
void oven_cooldown_init(oven_t *);

// oven state functions
void oven_idle(oven_t *);
void oven_preheating(oven_t *);
void oven_baking(oven_t *);
void oven_cooldown(oven_t *);

// oven functions
bool is_oven_state(oven_t *, oven_state_t);
void change_oven_state(oven_t *, oven_state_t);
oven_state_t get_oven_state(oven_t *);
uint32_t elapsed_s_since_oven_state(oven_t *, oven_state_t state);
oven_t oven_create();
void oven_loop(oven_t *);

static oven_state_node_t oven_sm[] = { // oven state machine
    {OVEN_S_IDLE, oven_idle, oven_idle_init},
    {OVEN_S_PREHEATING, oven_preheating, oven_preheating_init},
    {OVEN_S_BAKING, oven_baking, oven_baking_init},
    {OVEN_S_COOLDOWN, oven_cooldown, oven_cooldown_init}};

#endif // OVEN_STATE_H
