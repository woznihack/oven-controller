#pragma once

#ifndef OVEN_STATE_H
#define OVEN_STATE_H

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
  oven_state_t state;
  void (*stateFunc)(void);
  void (*initStateFunc)(void);
} oven_state_node_t;

// oven state functions
void oven_idle();
void oven_preheating();
void oven_baking();
void oven_cooldown();

// oven state init functions
void oven_idle_init();
void oven_preheating_init();
void oven_baking_init();
void oven_cooldown_init();

// helpers
bool is_oven_state(oven_state_t state);
void change_oven_state(oven_state_t state);
oven_state_t get_oven_state();
uint32_t elapsed_s_since_oven_state(oven_state_t state);

static oven_state_node_t oven_sm[] = { // oven state machine
    {OVEN_S_IDLE, oven_idle, oven_idle_init},
    {OVEN_S_PREHEATING, oven_preheating, oven_preheating_init},
    {OVEN_S_BAKING, oven_baking, oven_baking_init},
    {OVEN_S_COOLDOWN, oven_cooldown, oven_cooldown_init}};


#endif // OVEN_STATE_H
