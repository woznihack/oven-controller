
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

static uint32_t oven_get_temperature(oven_t *o){
  return (o->top_heater.current_temperature + o->deck_heater.current_temperature ) / 2;
}

void oven_idle_init(oven_t *o) {
  change_heater_state(&o->top_heater, HEATER_S_OFF);
  change_heater_state(&o->deck_heater, HEATER_S_OFF);
  o->data.current_phase = "idle";
}
void oven_idle(oven_t *o){}

void oven_preheating_init(oven_t *o)
{
  o->top_heater.target_temperature = baking_steps[0].temperature;
  o->deck_heater.target_temperature = baking_steps[0].temperature;

  change_heater_state(&o->top_heater, HEATER_S_ON);
  change_heater_state(&o->deck_heater, HEATER_S_ON);
  o->data.current_phase = "preheating";
  printf("[OVEN] PREHEAT started\n");
}

void oven_preheating(oven_t *o)
{
  o->data.temperature = oven_get_temperature(o);

  bool preheat_complete = (o->data.temperature >= baking_steps[0].temperature);
  if (preheat_complete)
  {
    change_oven_state(o, OVEN_S_BAKING);
    printf("[OVEN] PREHEAT complete\n");
  }
}

void oven_baking_init(oven_t *o)
{
  change_heater_state(&o->top_heater, HEATER_S_ON);
  change_heater_state(&o->deck_heater, HEATER_S_ON);
  o->data.current_phase = "baking";
  o->data.current_step = 0;
  printf("[OVEN] BAKING started\n");
}

void oven_baking(oven_t *o)
{
  uint32_t elapsed_s = elapsed_s_since_oven_state(o, OVEN_S_BAKING);
  uint32_t elapsed_m = elapsed_s / EMU_BAKING_SECONDS_PER_MINUTE;
  o->data.remaining_m = o->data.duration_m - elapsed_m;
  o->data.temperature = oven_get_temperature(o);

  if (o->data.remaining_m == 0)
  {
    change_oven_state(o, OVEN_S_COOLDOWN);
    change_heater_state(&o->top_heater, HEATER_S_OFF);
    change_heater_state(&o->deck_heater, HEATER_S_OFF);
    printf("[OVEN] BAKING complete\n");
  }
}

void oven_cooldown_init(oven_t *o)
{
  change_heater_state(&o->top_heater, HEATER_S_OFF);
  change_heater_state(&o->deck_heater, HEATER_S_OFF);
  o->data.current_phase = "cooldown";
  printf("[OVEN] COOLDOWN started\n");
}

void oven_cooldown(oven_t *o)
{
  o->data.temperature = oven_get_temperature(o);
  bool cooldown_complete = (o->data.temperature <= HEATER_TEMPERATURE_COOL);

  if (cooldown_complete)
  {
    change_oven_state(o, OVEN_S_IDLE);
    printf("[OVEN] COOLDOWN complete\n");
  }
}
