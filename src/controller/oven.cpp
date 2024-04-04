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

// private internal helpers
static time_t last_probes_read_time = 0;
static oven_state_t oven_get_state(oven_t *);

// oven state functions prototypes
static void oven_idle(oven_t *);
static void oven_preheating(oven_t *);
static void oven_baking(oven_t *);
static void oven_baking_paused(oven_t *);
static void oven_cooldown(oven_t *);
static void oven_probes_read(oven_t *);

static oven_state_node_t oven_sm[] = {  // oven state machine
    {OVEN_S_IDLE, oven_idle},
    {OVEN_S_PREHEATING, oven_preheating},
    {OVEN_S_BAKING, oven_baking},
    {OVEN_S_BAKING_PAUSED, oven_baking_paused},
    {OVEN_S_COOLDOWN, oven_cooldown}};

void initialize_heater_control(heater_control_t *hc) {
  hc->Kp = 1.0;
  hc->Ki = 0.5;
  hc->Kd = 0.05;
  hc->input = 20;
  hc->pid = new PID(&(hc->input), &(hc->output), &(hc->setpoint), hc->Kp, hc->Ki, hc->Kd, DIRECT);
  hc->pid->SetOutputLimits(0, OVEN_CONTROL_WINDOW_SIZE_MS);
  hc->pid->SetSampleTime(OVEN_PID_SAMPLE_TIME_MS);
  hc->pid->SetMode(AUTOMATIC);
}

double update_heater_control(heater_control_t *hc) {
  hc->pid->Compute();
  return hc->output;
}

oven_t oven_create() {
  oven_t oven;
  oven.current_state = OVEN_S_IDLE;
  oven.chamber_probe_reading = EMU_TEMP_INIT;
  oven.last_probe_read_ms = millis();
  oven.control_window_start_ms = millis();
  oven.program = NULL;

  initialize_heater_control(&oven.top_heater_control);
  initialize_heater_control(&oven.deck_heater_control);
  return oven;
}

oven_monitor_data_t oven_get_monitor_data(oven_t *o) {
  oven_monitor_data_t d;
  d.current_state = o->current_state;
  d.temperature = o->chamber_probe_reading;

  if (o->program != NULL && (o->current_state == OVEN_S_PREHEATING || o->current_state == OVEN_S_BAKING)) {
    baking_program_step_t *current_step = o->current_state == OVEN_S_PREHEATING ? &o->program->steps[0] : baking_program_current_step(o->program);
    if (current_step != NULL) {
      d.program_step_current = o->program->current_step_idx + 1;
      d.remaining_m = baking_program_current_step_remaining_m(o->program);
      d.program_steps_total = o->program->steps_count;
      d.config_duration_m = current_step->duration_m;
      d.config_temperature = current_step->temperature;
    }
  } else {
    d.program_step_current = UINT16_MAX;
    d.remaining_m = UINT16_MAX;
    d.program_steps_total = UINT16_MAX;
    d.config_duration_m = UINT16_MAX;
    d.config_temperature = UINT16_MAX;
  }

  return d;
}

void oven_read_probes(oven_t *o) {
  // TODO: read probes
  o->last_probe_read_ms = millis();
}

void oven_handle_actuators(oven_t *o) {
  uint32_t now = millis();

  if (o->current_state == OVEN_S_PREHEATING || o->current_state == OVEN_S_BAKING) {
    // reset the control window start time
    if ((now - o->control_window_start_ms) > OVEN_CONTROL_WINDOW_SIZE_MS) {
      debug("[OVEN] resetting control_window_start_ms\n");
      o->control_window_start_ms += OVEN_CONTROL_WINDOW_SIZE_MS;
    }

    o->top_heater_control.input = o->chamber_probe_reading;
    o->deck_heater_control.input = o->chamber_probe_reading;

    baking_program_step_t *current_step = o->current_state == OVEN_S_PREHEATING ? &o->program->steps[0] : baking_program_current_step(o->program);
    if (current_step != NULL) {
      o->top_heater_control.setpoint = current_step->temperature;
      o->deck_heater_control.setpoint = current_step->temperature;

      o->top_heater_control.pid->Compute();
      o->deck_heater_control.pid->Compute();


      if (current_step->top_heater_on && o->top_heater_control.output > (now - o->control_window_start_ms)) {
#if EMULATION
        o->chamber_probe_reading += EMU_TEMP_INC_PER_ON_CYCLE;
#else
// TODO: set PIN to on
#endif
      } else {
        // TODO: set PIN to off
      }

      if (current_step->deck_heater_on && o->deck_heater_control.output > (now - o->control_window_start_ms)) {
#if EMULATION
        o->chamber_probe_reading += EMU_TEMP_INC_PER_ON_CYCLE;
#else
// TODO: set PIN to on
#endif
      } else {
        // TODO: set PIN to off
      }


      debug("[OVEN] TOP PID Output: %.2f\n", o->top_heater_control.output);
      debug("[OVEN] TOP PID Input: %.2f\n", o->deck_heater_control.input);
      debug("[OVEN] TOP PID Set: %.2f\n", o->deck_heater_control.setpoint);
      debug("[OVEN] Chamber T: %.2f\n", o->chamber_probe_reading);
    }
  }

#if EMULATION
  //o->chamber_probe_reading = MAX(EMU_TEMP_COOL, o->chamber_probe_reading-EMU_TEMP_DECAY_PER_SECOND*OVEN_ACTUATOR_HANDLE_INTERVAL_MS/1000);
#endif
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
    if (o->current_state == OVEN_S_PREHEATING || o->current_state == OVEN_S_BAKING) {
      baking_program_step_t *current_step = o->current_state == OVEN_S_PREHEATING ? &o->program->steps[0] : baking_program_current_step(o->program);
    }
  }
}

static oven_state_t oven_get_state(oven_t *o) { return o->current_state; }

/* IDLE */
void oven_idle(oven_t *o) {}

/* PREHEATING */
void oven_preheating(oven_t *o) {
  baking_program_step_t program_step = o->program->steps[0];
  bool preheat_complete = (o->chamber_probe_reading >= program_step.temperature);

  if (preheat_complete) {
    printf("[OVEN] PREHEAT complete\n");
    oven_state_change(o, OVEN_S_BAKING);
  }
}

/* BAKING */
void oven_baking(oven_t *o) {
  // start or resume baking program
  if (!o->program->started) {
    debug("[OVEN] starting program\n");
    baking_program_start(o->program);
  } else if (o->program->paused) {
    baking_program_resume(o->program);
  }

  // if program is complete, go to cooldown else go to next step if current completed
  if (baking_program_is_complete(o->program)) {
    debug("[OVEN] completing program\n");
    baking_program_reset(o->program);
    oven_state_change(o, OVEN_S_COOLDOWN);
    return;
  } else if (baking_program_current_step_is_complete(o->program)) {
    debug("[OVEN] going to program next step \n");
    baking_program_goto_next_step(o->program);
  }
}

/* BAKING PAUSED */
void oven_baking_paused(oven_t *o) {
  if (!o->program->paused) {
    baking_program_pause(o->program);
  }
}

/* COOLDOWN */
void oven_cooldown(oven_t *o) {
  bool cooldown_complete = (o->chamber_probe_reading <= EMU_TEMP_COOL);
  if (cooldown_complete) {
    oven_state_change(o, OVEN_S_IDLE);
  }
}
