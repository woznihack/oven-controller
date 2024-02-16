#ifndef OVEN_STATE_H
#define OVEN_STATE_H

#include <stdint.h>
#include <time.h>

#include "../settings.h"
#include "../baking_program.h"
#include "heater.h"

// typedefs
typedef enum { OVEN_S_IDLE = 0, OVEN_S_PREHEATING, OVEN_S_BAKING, OVEN_S_COOLDOWN } oven_state_t;

typedef struct {
  bool light_on;
  bool fan_on;
  bool top_heater_on;
  bool deck_heater_on;
  bool grill_on;
  uint32_t avg_heater_temperature;
  uint32_t deck_heater_temperature;
  uint32_t top_heater_temperature;
  uint32_t remaining_m;
  oven_state_t current_state;

  uint16_t program_step_current;
  uint16_t program_steps_total;
  uint32_t config_temperature;
  uint32_t config_duration_m;
} oven_monitor_data_t;

typedef struct {
  oven_state_t current_state;
  time_t state_start_s[4];

  heater_t top_heater;
  heater_t deck_heater;

  uint16_t current_program_step;
  baking_program_t *program;
} oven_t;

static const char *oven_state_strings[] = {"IDLE", "PREHEATING", "BAKING", "COOLDOWN"};

// exposed functions
oven_t oven_create();
void oven_loop(oven_t *);
void oven_change_state(oven_t *, oven_state_t);
oven_monitor_data_t oven_get_monitor_data(oven_t);

#endif  // OVEN_STATE_H
