#ifndef OVEN_STATE_H
#define OVEN_STATE_H

#include <stdint.h>
#include <time.h>

#include "../shared_data_structures.h"
#include "heater.h"

// typedefs
typedef enum { OVEN_S_IDLE = 0, OVEN_S_PREHEATING, OVEN_S_BAKING, OVEN_S_COOLDOWN } oven_state_t;

typedef struct {
  oven_state_t current_state;
  time_t state_start_s[4];
  heater_t top_heater;
  heater_t deck_heater;

  oven_data_t data;

  uint16_t baking_steps_count;
  oven_data_t *baking_steps;
} oven_t;

static const char *oven_state_strings[] = {"IDLE", "PREHEATING", "BAKING", "COOLDOWN"};

// exposed functions
oven_t oven_create();
void oven_loop(oven_t *);
void oven_change_state(oven_t *, oven_state_t);
oven_state_t oven_get_state(oven_t *);

#endif  // OVEN_STATE_H
