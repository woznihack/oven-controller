#ifndef OVEN_STATE_H
#define OVEN_STATE_H

#include <stdint.h>

#include "../settings.h"
#include "../baking_program.h"
#include "../queue.h"
#include <PID_v1.h>

// typedefs
typedef enum { OVEN_S_IDLE = 0, OVEN_S_PREHEATING, OVEN_S_BAKING, OVEN_S_BAKING_PAUSED, OVEN_S_COOLDOWN } oven_state_t;

typedef struct {
  bool light_on;
  bool fan_on;
  bool top_heater_on;
  bool deck_heater_on;
  uint32_t temperature;
  uint32_t remaining_m;
  oven_state_t current_state;

  uint16_t program_step_current;
  uint16_t program_steps_total;
  uint32_t config_temperature;
  uint32_t config_duration_m;
} oven_monitor_data_t;

typedef struct {
  oven_state_t current_state;
  uint32_t state_start_ms[4];

  baking_program_t *program;
  baking_program_step_t *program_current_step;

  uint32_t last_probe_read_ms;
  uint32_t control_window_start_ms;
  double chamber_probe_reading;

  PID *top_heater_pid;
  PID *deck_heater_pid;
  double pid_setpoint;
  double top_heater_pid_output;
  double deck_heater_pid_output;
} oven_t;

static const char *oven_state_strings[] = {"IDLE", "PREHEATING", "BAKING", "PAUSED", "COOLDOWN"};

// exposed functions
oven_t oven_create();
void oven_init_controllers(oven_t *);
void oven_read_probes(oven_t *);
void oven_handle_state(oven_t *);
void oven_handle_actuators(oven_t *);
void oven_handle_control_message(oven_t *, q_node_t);
void oven_state_change(oven_t *, oven_state_t);
oven_monitor_data_t oven_get_monitor_data(oven_t *);

#endif  // OVEN_STATE_H
