#include "oven.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../main.h"
#include "Arduino.h"
#include "controller.h"

typedef struct {
  oven_state_t state;
  void (*stateFunc)(oven_t *);
} oven_state_node_t;

// oven state functions prototypes
static oven_state_t oven_get_state(oven_t *);
static void oven_idle(oven_t *);
static void oven_preheating(oven_t *);
static void oven_baking(oven_t *);
static void oven_baking_paused(oven_t *);
static void oven_cooldown(oven_t *);

// oven state machine
static oven_state_node_t oven_sm[] = {{OVEN_S_IDLE, oven_idle}, {OVEN_S_PREHEATING, oven_preheating}, {OVEN_S_BAKING, oven_baking}, {OVEN_S_BAKING_PAUSED, oven_baking_paused}, {OVEN_S_COOLDOWN, oven_cooldown}};

oven_t oven_create() {
  oven_t oven;
  oven.program = NULL;
  oven.program_current_step = NULL;
  oven.current_state = OVEN_S_IDLE;
  oven.control_window_start_ms = millis();
  oven.chamber_probe_reading = EMU_TEMP_INIT;
  oven.pid_setpoint = EMU_TEMP_INIT;
  return oven;
}

/**
 * Controllers should be initialized after the oven_create, otherwise the references
 * for the PID input, output and setpoint variables addresse will be wrong!
 * (the object initialized in oven_create has a different address than the one in the context of calling the create)
 */
void oven_init_controllers(oven_t *o) {
  o->top_heater_pid = new PID(&o->chamber_probe_reading, &o->top_heater_pid_output, &o->pid_setpoint, 100, 0.5, 0.05, DIRECT);
  o->top_heater_pid->SetOutputLimits(0, OVEN_CONTROL_WINDOW_SIZE_MS);
  o->top_heater_pid->SetSampleTime(OVEN_CONTROL_PID_SAMPLE_TIME_MS);
  o->top_heater_pid->SetMode(AUTOMATIC);

  o->deck_heater_pid = new PID(&o->chamber_probe_reading, &o->deck_heater_pid_output, &o->pid_setpoint, 100, 0.5, 0.05, DIRECT);
  o->deck_heater_pid->SetOutputLimits(0, OVEN_CONTROL_WINDOW_SIZE_MS);
  o->deck_heater_pid->SetSampleTime(OVEN_CONTROL_PID_SAMPLE_TIME_MS);
  o->deck_heater_pid->SetMode(AUTOMATIC);
}

oven_monitor_data_t oven_get_monitor_data(oven_t *o) {
  oven_monitor_data_t d;
  d.current_state = o->current_state;
  d.temperature = o->chamber_probe_reading;

  if (o->program_current_step != NULL) {
    d.program_step_current = o->program->current_step_idx + 1;
    d.program_steps_total = o->program->steps_count;
    d.remaining_m = baking_program_current_step_remaining_m(o->program);
    d.config_duration_m = o->program_current_step->duration_m;
    d.config_temperature = o->program_current_step->temperature;
    d.fan_on = o->program_current_step->fan_on;
    d.top_heater_on = o->program_current_step->top_heater_on;
    d.deck_heater_on = o->program_current_step->deck_heater_on;
    d.light_on = o->program_current_step->light_on;
  } else {
    d.program_step_current = UINT16_MAX;
    d.remaining_m = UINT16_MAX;
    d.program_steps_total = UINT16_MAX;
    d.config_duration_m = UINT16_MAX;
    d.config_temperature = UINT16_MAX;
    d.fan_on = false;
    d.top_heater_on = false;
    d.deck_heater_on = false;
    d.light_on = false;
  }

  return d;
}

void oven_handle_control_message(oven_t *o, q_node_t data) {
  // oven state change controls
  if (data.event == CONTROL_OVEN_START) {
    oven_state_change(o, OVEN_S_PREHEATING);
  }
  if (data.event == CONTROL_OVEN_PAUSE) {
    oven_state_change(o, OVEN_S_BAKING_PAUSED);
  }
  if (data.event == CONTROL_OVEN_RESUME) {
    oven_state_change(o, OVEN_S_BAKING);
  }
  if (data.event == CONTROL_OVEN_STOP) {
    oven_state_change(o, OVEN_S_IDLE);
  }

  // initial setup of the baking program
  if (data.event == CONTROL_OVEN_SET_BAKING_PROGRAM) {
    o->program = (baking_program_t *)data.payload;
  }

  // manage program overrides from UI
  if (o->program != NULL && o->program_current_step != NULL) {
    baking_program_step_t *curr_step = o->program_current_step;
    if (data.event == CONTROL_OVEN_SET_TEMPERATURE) {
      curr_step->temperature = *(uint32_t *)data.payload;
    }
    if (data.event == CONTROL_OVEN_SET_DURATION_M) {
      curr_step->duration_m = *(uint32_t *)data.payload;
    }
    if (data.event == CONTROL_OVEN_LIGHT_ON) {
      curr_step->light_on = true;
    }
    if (data.event == CONTROL_OVEN_LIGHT_OFF) {
      curr_step->light_on = false;
    }
    if (data.event == CONTROL_OVEN_FAN_ON) {
      curr_step->fan_on = true;
    }
    if (data.event == CONTROL_OVEN_FAN_OFF) {
      curr_step->fan_on = false;
    }
    if (data.event == CONTROL_OVEN_TOP_HEATER_ON) {
      curr_step->top_heater_on = true;
    }
    if (data.event == CONTROL_OVEN_TOP_HEATER_OFF) {
      if (curr_step->deck_heater_on) {
        curr_step->top_heater_on = false;
      }
      else {
        debug("[OVEN] Can't turn OFF top heater!");
      }
    }
    if (data.event == CONTROL_OVEN_DECK_HEATER_ON) {
      curr_step->deck_heater_on = true;
    }
    if (data.event == CONTROL_OVEN_DECK_HEATER_OFF) {
      if (curr_step->top_heater_on) {
        curr_step->deck_heater_on = false;
      }
      else {
        debug("[OVEN] Can't turn OFF deck heater!");
      }
    }


  }
}

void oven_read_probes(oven_t *o) {
  // TODO: read probes
}

void oven_handle_actuators(oven_t *o) {
  uint32_t now = millis();

  if (o->current_state == OVEN_S_PREHEATING || o->current_state == OVEN_S_BAKING) {
    // reset the control window start time
    if ((now - o->control_window_start_ms) > OVEN_CONTROL_WINDOW_SIZE_MS) {
      o->control_window_start_ms += OVEN_CONTROL_WINDOW_SIZE_MS;
    }

    // if it's a baking state
    if (o->program_current_step != NULL) {
      // adjust the setpoint (it can be changed from UI)
      o->pid_setpoint = o->program_current_step->temperature * 1.0;

      // compute pid calculations
      o->top_heater_pid->Compute();
      o->deck_heater_pid->Compute();

      // pid output is (0, OVEN_CONTROL_WINDOW_SIZE_MS), and represents the
      // % of OVEN_CONTROL_WINDOW_SIZE_MS for the heater to be on

      if (o->program_current_step->top_heater_on && o->top_heater_pid_output > (now - o->control_window_start_ms)) {
#if EMULATION
        // during emulation we just increase the chamber_probe_reading by a fixed amount
        o->chamber_probe_reading += EMU_TEMP_INC_PER_ON_CYCLE;
#else
        // TODO: set TOP heater PIN to on
#endif
      } else {
        // TODO: set TOP heater PIN to off
      }

      if (o->program_current_step->deck_heater_on && o->deck_heater_pid_output > (now - o->control_window_start_ms)) {
#if EMULATION
        // during emulation we just increase the chamber_probe_reading by a fixed amount
        o->chamber_probe_reading += EMU_TEMP_INC_PER_ON_CYCLE;
#else
        // TODO: set PIN to on
#endif
      } else {
        // TODO: set PIN to off
      }
    }

#if EMULATION
    // if in baking phase, to keep the PID work, emulate the temperature decrease substracting an amount depending on the loop time
    o->chamber_probe_reading = MAX(EMU_TEMP_INIT, o->chamber_probe_reading - EMU_TEMP_DECAY_PER_SECOND / 10 * OVEN_ACTUATOR_HANDLING_INTERVAL_MS / 1000);
#endif

  } else {
#if EMULATION
    // if not in baking phase, emulate the temperature decrease substracting an amount depending on the loop time
    o->chamber_probe_reading = MAX(EMU_TEMP_INIT, o->chamber_probe_reading - EMU_TEMP_DECAY_PER_SECOND * OVEN_ACTUATOR_HANDLING_INTERVAL_MS / 1000);
#endif
  }
}

/**** STATE HANDLING AND STATE FUNCTIONS ****/

void oven_handle_state(oven_t *o) { (oven_sm[oven_get_state(o)].stateFunc)(o); }

void oven_state_change(oven_t *o, oven_state_t s) {
  if (o->current_state == s) return;

  // can be different than IDLE only if program is set!
  if (s == OVEN_S_IDLE || o->program != NULL) {
    printf("[OVEN] switching state %s -> %s\n", oven_state_strings[o->current_state], oven_state_strings[s]);
    o->state_start_ms[s] = millis();
    o->current_state = s;
  }
}

static oven_state_t oven_get_state(oven_t *o) { return o->current_state; }

void oven_idle(oven_t *o) { o->program_current_step = NULL; }

void oven_preheating(oven_t *o) {
  o->program_current_step = baking_program_get_step(o->program, 0);
  bool preheat_complete = (o->chamber_probe_reading >= o->program_current_step->temperature);
  if (preheat_complete) {
    oven_state_change(o, OVEN_S_BAKING);
  }
}

void oven_baking(oven_t *o) {
  // start or resume baking program
  if (!o->program->started) {
    baking_program_start(o->program);
    o->program_current_step = baking_program_get_current_step(o->program);
  } else if (o->program->paused) {
    baking_program_resume(o->program);
  }

  // if program is complete, go to cooldown else go to next step if current completed
  if (baking_program_is_complete(o->program)) {
    baking_program_reset(o->program);
    oven_state_change(o, OVEN_S_COOLDOWN);
    return;
  } else if (baking_program_current_step_is_complete(o->program)) {
    baking_program_goto_next_step(o->program);
    o->program_current_step = baking_program_get_current_step(o->program);
  }
}

void oven_baking_paused(oven_t *o) {
  if (!o->program->paused) {
    baking_program_pause(o->program);
  }
}

void oven_cooldown(oven_t *o) {
  o->program_current_step = NULL;
  bool cooldown_complete = (o->chamber_probe_reading <= OVEN_TEMP_COOL);
  if (cooldown_complete) {
    oven_state_change(o, OVEN_S_IDLE);
  }
}
