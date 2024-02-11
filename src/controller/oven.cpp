
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "controller.h"
#include "oven.h"

extern oven_data_t *baking_steps;
extern uint16_t baking_steps_count;

oven_t oven_create()
{
  oven_t oven;
  oven.current_state = OVEN_S_IDLE;
  oven.top_heater = heater_create();
  oven.deck_heater = heater_create();
  oven.data.current_step = UINT16_MAX;
  return oven;
}

void oven_loop(oven_t *o)
{
  (oven_sm[get_oven_state(o)].stateFunc)(o);
  heater_loop(&o->top_heater);
  heater_loop(&o->deck_heater);
}

bool is_oven_state(oven_t *o, oven_state_t s)
{
  return o->current_state == s;
}

void change_oven_state(oven_t *o, oven_state_t s)
{
  (oven_sm[s].initStateFunc)(o);
  o->state_start_s[s] = time(NULL);
  o->current_state = s;
}

oven_state_t get_oven_state(oven_t *o)
{
  return o->current_state;
}

uint32_t elapsed_s_since_oven_state(oven_t *o, oven_state_t s)
{
  return difftime(time(NULL), o->state_start_s[s]);
}

static void oven_update_temperature_from_probes(oven_t *o)
{
  //  for now we suppose 2 probes, let's do the average
  o->data.temperature = (o->top_heater.current_temperature + o->deck_heater.current_temperature) / 2;
}


// switches to next baking steps and returns true if there are more, returns false otherwise
static bool oven_goto_next_baking_step(oven_t *o){
  uint16_t next_step = o->data.current_step == UINT16_MAX ? 0 : (o->data.current_step + 1) ;
  if (next_step >= baking_steps_count) {
    return false;
  }

  o->data = baking_steps[next_step];
  o->data.current_step = next_step;
  o->data.total_steps = baking_steps_count;

  o->top_heater.target_temperature = o->data.temperature;
  o->deck_heater.target_temperature = o->data.temperature;

  printf("[OVEN] baking step change %d/%d\n", o->data.current_step, o->data.total_steps);
  printf("[OVEN] settings T=%d, D_M=%d\n", o->data.temperature, o->data.duration_m);
  return true;
}

/* IDLE */
void oven_idle_init(oven_t *o)
{
  change_heater_state(&o->top_heater, HEATER_S_OFF);
  change_heater_state(&o->deck_heater, HEATER_S_OFF);
  o->data.current_phase = "idle";
}
void oven_idle(oven_t *o) {}

/* PREHEATING */
void oven_preheating_init(oven_t *o)
{
  o->top_heater.target_temperature = baking_steps[0].temperature;
  o->deck_heater.target_temperature = baking_steps[0].temperature;
  change_heater_state(&o->top_heater, HEATER_S_ON);
  change_heater_state(&o->deck_heater, HEATER_S_ON);
  o->data.current_phase = "preheating";
}

void oven_preheating(oven_t *o)
{
  oven_update_temperature_from_probes(o);

  bool preheat_complete = (o->data.temperature >= baking_steps[0].temperature);
  if (preheat_complete)
  {
    printf("[OVEN] PREHEAT complete\n");
    change_oven_state(o, OVEN_S_BAKING);
  }
}

/* BAKING */
void oven_baking_init(oven_t *o)
{
  printf("[OVEN] BAKING started\n");
  change_heater_state(&o->top_heater, HEATER_S_ON);
  change_heater_state(&o->deck_heater, HEATER_S_ON);
  o->data.current_phase = "baking";
  oven_goto_next_baking_step(o);
}

void oven_baking(oven_t *o)
{
  oven_update_temperature_from_probes(o);

  uint32_t elapsed_s = elapsed_s_since_oven_state(o, OVEN_S_BAKING);

#ifdef EMULATION
  uint32_t elapsed_m = elapsed_s / EMU_BAKING_SECONDS_PER_MINUTE;
#else
  uint32_t elapsed_m = elapsed_s / 60;
#endif

  o->data.remaining_m = o->data.duration_m - elapsed_m;

  if (o->data.remaining_m == 0)
  {
    if (!oven_goto_next_baking_step(o)) {
      printf("[OVEN] BAKING complete\n");
      change_oven_state(o, OVEN_S_COOLDOWN);
      change_heater_state(&o->top_heater, HEATER_S_OFF);
      change_heater_state(&o->deck_heater, HEATER_S_OFF);
    }
    else {
      printf("[OVEN] BAKING next step\n");
    }
  }
}

void oven_cooldown_init(oven_t *o)
{
  printf("[OVEN] COOLDOWN started\n");
  change_heater_state(&o->top_heater, HEATER_S_OFF);
  change_heater_state(&o->deck_heater, HEATER_S_OFF);
  o->data.current_phase = "cooldown";
}

void oven_cooldown(oven_t *o)
{
  oven_update_temperature_from_probes(o);

  bool cooldown_complete = (o->data.temperature <= HEATER_TEMPERATURE_COOL);
  if (cooldown_complete)
  {
    printf("[OVEN] COOLDOWN complete\n");
    change_oven_state(o, OVEN_S_IDLE);
  }
}
