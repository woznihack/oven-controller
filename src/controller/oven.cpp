
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "controller.h"
#include "oven.h"

oven_t oven_create(){
  oven_t oven;
  oven.current_state = OVEN_S_IDLE;
  oven.top_heater = heater_create();
  oven.deck_heater = heater_create();
  return oven;
}

void oven_loop(oven_t *o){
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
  o->current_state = s;
  o->state_start_s[s] = time(NULL);
  (oven_sm[o->current_state].initStateFunc)(o);
}

oven_state_t get_oven_state(oven_t *o)
{
  return o->current_state;
}

uint32_t elapsed_s_since_oven_state(oven_t *o, oven_state_t s)
{
  return difftime(time(NULL), o->state_start_s[s]);
}

void oven_idle_init(oven_t *o) {
  change_heater_state(&o->top_heater, HEATER_S_OFF);
  change_heater_state(&o->deck_heater, HEATER_S_OFF);
}
void oven_idle(oven_t *o){}

void oven_preheating_init(oven_t *o)
{
  o->top_heater.target_temperature = baking_temperature;
  o->deck_heater.target_temperature = baking_temperature;

  change_heater_state(&o->top_heater, HEATER_S_ON);
  change_heater_state(&o->deck_heater, HEATER_S_ON);
  q_enqueue(ui_queue, UI_UPDATE_BAKING_PHASE, "preheating");
  printf("[OVEN] PREHEAT started\n");
}

void oven_preheating(oven_t *o)
{
  bool preheat_complete = (o->top_heater.current_temperature >= baking_temperature &&
                           o->deck_heater.current_temperature >= baking_temperature);
  if (preheat_complete)
  {
    change_oven_state(o, OVEN_S_BAKING);
    // tell UI preheat complete
    printf("[OVEN] PREHEAT complete\n");
  }
}

void oven_baking_init(oven_t *o)
{
  change_heater_state(&o->top_heater, HEATER_S_ON);
  change_heater_state(&o->deck_heater, HEATER_S_ON);
  q_enqueue(ui_queue, UI_UPDATE_BAKING_PHASE, "baking");
  printf("[OVEN] BAKING started\n");
}

void oven_baking(oven_t *o)
{
#ifdef EMULATION
  uint32_t elapsed_s = elapsed_s_since_oven_state(o, OVEN_S_BAKING);
  uint32_t elapsed_m = elapsed_s / EMU_BAKING_SECONDS_PER_MINUTE;
  baking_remaining_m = baking_duration_m - elapsed_m;
#endif

  if (baking_remaining_m == 0)
  {
    change_oven_state(o, OVEN_S_COOLDOWN);
    change_heater_state(&o->top_heater, HEATER_S_OFF);
    change_heater_state(&o->deck_heater, HEATER_S_OFF);
    printf("[OVEN] BAKING complete\n");
  }

  q_enqueue(ui_queue, UI_UPDATE_REMAINING_M, &baking_remaining_m);
}

void oven_cooldown_init(oven_t *o)
{
  change_heater_state(&o->top_heater, HEATER_S_OFF);
  change_heater_state(&o->deck_heater, HEATER_S_OFF);
  q_enqueue(ui_queue, UI_UPDATE_BAKING_PHASE, "cooldown");
  printf("[OVEN] COOLDOWN started\n");
}

void oven_cooldown(oven_t *o)
{
  bool cooldown_complete = (o->top_heater.current_temperature <= HEATER_TEMPERATURE_COOL &&
                            o->deck_heater.current_temperature <= HEATER_TEMPERATURE_COOL);
  if (cooldown_complete)
  {
    change_oven_state(o, OVEN_S_IDLE);
    printf("[OVEN] COOLDOWN complete\n");
  }
}
