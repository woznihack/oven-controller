#include "oven.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../main.h"
#include "controller.h"

typedef struct {
  oven_state_t state;
  void (*stateFunc)(oven_t *);
  void (*initStateFunc)(oven_t *);
} oven_state_node_t;

// private internal helpers
static uint32_t elapsed_m_since_oven_state(oven_t *, oven_state_t state);
static oven_state_t oven_get_state(oven_t *);

// oven state init functions
static void oven_idle_init(oven_t *);
static void oven_preheating_init(oven_t *);
static void oven_baking_init(oven_t *);
static void oven_cooldown_init(oven_t *);

// oven state functions
static void oven_idle(oven_t *);
static void oven_preheating(oven_t *);
static void oven_baking(oven_t *);
static void oven_cooldown(oven_t *);

static oven_state_node_t oven_sm[] = {  // oven state machine
    {OVEN_S_IDLE, oven_idle, oven_idle_init},
    {OVEN_S_PREHEATING, oven_preheating, oven_preheating_init},
    {OVEN_S_BAKING, oven_baking, oven_baking_init},
    {OVEN_S_COOLDOWN, oven_cooldown, oven_cooldown_init}};

oven_t oven_create() {
  oven_t oven;
  oven.current_state = OVEN_S_IDLE;

  oven.top_heater = heater_create();
  oven.deck_heater = heater_create();

  oven.current_program_step = UINT16_MAX;
  oven.program = NULL;
  return oven;
}

void oven_loop(oven_t *o) {
  (oven_sm[oven_get_state(o)].stateFunc)(o);
  heater_loop(&o->top_heater);
  heater_loop(&o->deck_heater);
}

void oven_change_state(oven_t *o, oven_state_t s) {
  // can be different than IDLE only if program is set!
  if (s == OVEN_S_IDLE || o->program != NULL) {
    printf("[OVEN] Going to next state %s\n", oven_state_strings[s]);
    (oven_sm[s].initStateFunc)(o);
    o->state_start_s[s] = time(NULL);
    o->current_state = s;
  }
}

oven_monitor_data_t oven_get_monitor_data(oven_t o) {
  oven_monitor_data_t d;
  d.current_state = o.current_state;
  d.avg_heater_temperature = (o.top_heater.current_temperature + o.deck_heater.current_temperature) / 2;
  d.top_heater_temperature = o.top_heater.current_temperature;
  d.deck_heater_temperature = o.deck_heater.current_temperature;

  if (o.program != NULL && o.program->steps_count > 0) {
    d.program_step_current = o.current_program_step + 1;
    d.program_steps_total = o.program->steps_count;
    baking_program_step_t current_step = o.program->steps[o.current_program_step];
    d.remaining_m = (o.current_state == OVEN_S_BAKING) ? (current_step.duration_m - elapsed_m_since_oven_state(&o, OVEN_S_BAKING)) : 0;
    d.light_on = current_step.light_on;
    d.fan_on = current_step.fan_on;
    d.top_heater_on = current_step.top_heater_on;
    d.deck_heater_on = current_step.deck_heater_on;
    d.grill_on = current_step.grill_on;
    d.config_duration_m = current_step.duration_m;
    d.config_temperature = current_step.top_heater_temperature;
  }

  return d;
}

static oven_state_t oven_get_state(oven_t *o) { return o->current_state; }

static uint32_t elapsed_m_since_oven_state(oven_t *o, oven_state_t s) {
  uint32_t elapsed_s = difftime(time(NULL), o->state_start_s[s]);

#ifdef EMULATION
  uint32_t elapsed_m = elapsed_s / EMU_BAKING_SECONDS_PER_MINUTE;
#else
  uint32_t elapsed_m = elapsed_s / 60;
#endif
  return elapsed_m;
}

// attempts to go to the next baking step if existent
// returns the attempt result
static bool oven_goto_next_baking_step(oven_t *o) {
  // program is not set, can't go to next baking step!
  if (o->program == NULL) {
    return false;
  }

  uint16_t next_step_idx = o->current_program_step == UINT16_MAX ? 0 : (o->current_program_step + 1);
  if (next_step_idx >= o->program->steps_count) {
    return false;
  }
  o->current_program_step = next_step_idx;
  debug("[OVEN] baking goto next step (%d/%d)\n", o->current_program_step + 1, o->program->steps_count);
  return true;
}

/* IDLE */
void oven_idle_init(oven_t *o) {
  change_heater_state(&o->top_heater, HEATER_S_OFF);
  change_heater_state(&o->deck_heater, HEATER_S_OFF);
}

void oven_idle(oven_t *o) {
  change_heater_state(&o->top_heater, HEATER_S_OFF);
  change_heater_state(&o->deck_heater, HEATER_S_OFF);
}

/* PREHEATING */
void oven_preheating_init(oven_t *o) {
  o->current_program_step = UINT16_MAX;
  oven_goto_next_baking_step(o);
  change_heater_state(&o->top_heater, HEATER_S_ON);
  change_heater_state(&o->deck_heater, HEATER_S_ON);
}

void oven_preheating(oven_t *o) {
  baking_program_step_t program_step = o->program->steps[0];
  o->top_heater.target_temperature = program_step.top_heater_temperature;
  o->deck_heater.target_temperature = program_step.deck_heater_temperature;

  bool preheat_complete = (o->top_heater.current_temperature >= program_step.top_heater_temperature && o->deck_heater.current_temperature >= program_step.deck_heater_temperature);

  if (preheat_complete) {
    printf("[OVEN] TopHeater  t vs set point %d of %d\n", o->top_heater.current_temperature, program_step.top_heater_temperature);
    printf("[OVEN] DeckHeater t vs set point %d of %d\n", o->deck_heater.current_temperature, program_step.deck_heater_temperature);
    printf("[OVEN] PREHEAT complete\n");
    oven_change_state(o, OVEN_S_BAKING);
  }
}

/* BAKING */
void oven_baking_init(oven_t *o) {
  change_heater_state(&o->top_heater, HEATER_S_ON);
  change_heater_state(&o->deck_heater, HEATER_S_ON);
}
void oven_baking(oven_t *o) {
  baking_program_step_t program_step = o->program->steps[o->current_program_step];

  o->top_heater.target_temperature = program_step.top_heater_temperature;
  o->deck_heater.target_temperature = program_step.deck_heater_temperature;

  uint16_t remaining_m = program_step.duration_m - elapsed_m_since_oven_state(o, OVEN_S_BAKING);

  if (remaining_m <= 0) {
    if (!oven_goto_next_baking_step(o))
      oven_change_state(o, OVEN_S_COOLDOWN);
    else
      oven_change_state(o, OVEN_S_BAKING);
  }
}

void oven_cooldown_init(oven_t *o) {
  change_heater_state(&o->top_heater, HEATER_S_OFF);
  change_heater_state(&o->deck_heater, HEATER_S_OFF);
}

void oven_cooldown(oven_t *o) {
  bool cooldown_complete = (o->top_heater.current_temperature <= HEATER_TEMPERATURE_COOL && o->deck_heater.current_temperature <= HEATER_TEMPERATURE_COOL);
  if (cooldown_complete) {
    oven_change_state(o, OVEN_S_IDLE);
  }
}
