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
static uint32_t elapsed_s_since_oven_state(oven_t *, oven_state_t state);
static bool is_oven_state(oven_t *, oven_state_t);

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
  oven.data.current_step = UINT16_MAX;
  return oven;
}

void oven_loop(oven_t *o) {
  (oven_sm[oven_get_state(o)].stateFunc)(o);
  heater_loop(&o->top_heater);
  heater_loop(&o->deck_heater);
}

void oven_change_state(oven_t *o, oven_state_t s) {
  (oven_sm[s].initStateFunc)(o);
  o->state_start_s[s] = time(NULL);
  o->current_state = s;
}

oven_state_t oven_get_state(oven_t *o) { return o->current_state; }

static bool is_oven_state(oven_t *o, oven_state_t s) { return o->current_state == s; }
static uint32_t elapsed_s_since_oven_state(oven_t *o, oven_state_t s) { return difftime(time(NULL), o->state_start_s[s]); }

static void oven_update_temperature_from_probes(oven_t *o) {
  //  for now we suppose 2 probes, let's do the average
  o->data.temperature = (o->top_heater.current_temperature + o->deck_heater.current_temperature) / 2;
}

// switches to next baking steps and returns true if there are more, returns
// false otherwise
static bool oven_goto_next_baking_step(oven_t *o) {
  uint16_t next_step_idx = o->data.current_step == UINT16_MAX ? 0 : (o->data.current_step + 1);
  if (next_step_idx >= o->baking_steps_count) {
    return false;
  }

  oven_data_t next_step = o->baking_steps[next_step_idx];

  // update current fields with next step ones
  o->data.current_step = next_step_idx;
  o->data.temperature_set = next_step.temperature_set;
  o->data.duration_m_set = next_step.duration_m_set;
  o->data.light_on = next_step.light_on;
  o->data.fan_on = next_step.fan_on;
  o->data.top_heater_on = next_step.top_heater_on;
  o->data.deck_heater_on = next_step.deck_heater_on;
  o->data.grill_on = next_step.grill_on;

  // reset remaining_m
  o->data.remaining_m = 0;

  // update heaters SET point
  o->top_heater.target_temperature = o->data.temperature_set;
  o->deck_heater.target_temperature = o->data.temperature_set;

  debug("[OVEN] baking goto next step (%d/%d)\n", o->data.current_step + 1, o->data.total_steps);
  return true;
}

/* IDLE */
void oven_idle_init(oven_t *o) {
  change_heater_state(&o->top_heater, HEATER_S_OFF);
  change_heater_state(&o->deck_heater, HEATER_S_OFF);
  o->data.current_phase = "idle";
}

void oven_idle(oven_t *o) {}

/* PREHEATING */
void oven_preheating_init(oven_t *o) {
  o->data.temperature_set = o->baking_steps[0].temperature_set;
  o->top_heater.target_temperature = o->baking_steps[0].temperature_set;
  o->deck_heater.target_temperature = o->baking_steps[0].temperature_set;
  change_heater_state(&o->top_heater, HEATER_S_ON);
  change_heater_state(&o->deck_heater, HEATER_S_ON);
  o->data.current_phase = "preheating";
}

void oven_preheating(oven_t *o) {
  oven_update_temperature_from_probes(o);

  bool preheat_complete = (o->data.temperature >= o->baking_steps[0].temperature_set);
  if (preheat_complete) {
    printf("[OVEN] PREHEAT complete\n");
    oven_change_state(o, OVEN_S_BAKING);
  }
}

/* BAKING */
void oven_baking_init(oven_t *o) {
  printf("[OVEN] BAKING started\n");
  change_heater_state(&o->top_heater, HEATER_S_ON);
  change_heater_state(&o->deck_heater, HEATER_S_ON);
  o->data.current_phase = "baking";
  oven_goto_next_baking_step(o);
}

void oven_baking(oven_t *o) {
  oven_update_temperature_from_probes(o);

  uint32_t elapsed_s = elapsed_s_since_oven_state(o, OVEN_S_BAKING);

#ifdef EMULATION
  uint32_t elapsed_m = elapsed_s / EMU_BAKING_SECONDS_PER_MINUTE;
#else
  uint32_t elapsed_m = elapsed_s / 60;
#endif

  o->data.remaining_m = o->data.duration_m_set - elapsed_m;

  if (o->data.remaining_m == 0) {
    if (!oven_goto_next_baking_step(o)) {
      printf("[OVEN] BAKING complete\n");
      oven_change_state(o, OVEN_S_COOLDOWN);
      change_heater_state(&o->top_heater, HEATER_S_OFF);
      change_heater_state(&o->deck_heater, HEATER_S_OFF);
    } else {
      printf("[OVEN] BAKING next step\n");
    }
  }
}

void oven_cooldown_init(oven_t *o) {
  printf("[OVEN] COOLDOWN started\n");
  change_heater_state(&o->top_heater, HEATER_S_OFF);
  change_heater_state(&o->deck_heater, HEATER_S_OFF);
  o->data.current_phase = "cooldown";
}

void oven_cooldown(oven_t *o) {
  oven_update_temperature_from_probes(o);

  bool cooldown_complete = (o->data.temperature <= HEATER_TEMPERATURE_COOL);
  if (cooldown_complete) {
    printf("[OVEN] COOLDOWN complete\n");
    oven_change_state(o, OVEN_S_IDLE);
  }
}
