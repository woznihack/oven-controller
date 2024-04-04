#ifndef OVEN_STATE_H
#define OVEN_STATE_H

#include <stdint.h>

#include "../settings.h"
#include "../baking_program.h"
#include <PID_v1.h>

// typedefs
typedef enum { OVEN_S_IDLE = 0, OVEN_S_PREHEATING, OVEN_S_BAKING, OVEN_S_BAKING_PAUSED, OVEN_S_COOLDOWN } oven_state_t;

typedef struct {
  bool light_on;
  bool fan_on;
  bool top_heater_on;
  bool deck_heater_on;
  bool grill_on;
  uint32_t temperature;
  uint32_t remaining_m;
  oven_state_t current_state;

  uint16_t program_step_current;
  uint16_t program_steps_total;
  uint32_t config_temperature;
  uint32_t config_duration_m;
} oven_monitor_data_t;

typedef struct {
  PID *pid;
  double Kp;
  double Ki;
  double Kd;
  double input;
  double output;
  double setpoint;
} heater_control_t;

typedef struct {
  oven_state_t current_state;
  uint32_t state_start_ms[4];

  baking_program_t *program;

  uint32_t last_probe_read_ms;
  uint32_t control_window_start_ms;
  uint16_t chamber_probe_reading;

  heater_control_t top_heater_control;
  heater_control_t deck_heater_control;
} oven_t;

static const char *oven_state_strings[] = {"IDLE", "PREHEATING", "BAKING", "PAUSED", "COOLDOWN"};

// exposed functions
oven_t oven_create();
void oven_read_probes(oven_t *);
void oven_handle_state(oven_t *);
void oven_handle_actuators(oven_t *);
void oven_state_change(oven_t *, oven_state_t);
oven_monitor_data_t oven_get_monitor_data(oven_t *);

#endif  // OVEN_STATE_H
