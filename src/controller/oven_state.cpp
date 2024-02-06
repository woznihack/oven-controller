
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


#include "controller.h"
#include "oven_state.h"


static oven_state_t current_oven_state = OVEN_S_IDLE;
static time_t oven_state_start_s[4];

bool is_oven_state(oven_state_t state)
{
  return state == current_oven_state;
}

void change_oven_state(oven_state_t state)
{
  current_oven_state = state;
  oven_state_start_s[state] = time(NULL);
  (oven_sm[current_oven_state].initStateFunc)();
}

oven_state_t get_oven_state(){
  return current_oven_state;
}

uint32_t elapsed_s_since_oven_state(oven_state_t state){
  return difftime(time(NULL), oven_state_start_s[state]);
}

void oven_idle_init() {

}

void oven_idle()
{
  current_top_heater_state = OVEN_S_TOP_HEATER_OFF;
  current_deck_heater_state = OVEN_S_DECK_HEATER_OFF;
}

void oven_preheating_init()
{
  current_top_heater_state = OVEN_S_TOP_HEATER_ON;
  current_deck_heater_state = OVEN_S_DECK_HEATER_ON;
  q_enqueue(ui_queue, UI_UPDATE_BAKING_PHASE, "preheating");
  printf("[OVEN] PREHEAT started\n");
}

void oven_preheating()
{
  bool preheat_complete = (top_heater_temperature >= baking_temperature &&
                           deck_heater_temperature >= baking_temperature);
  if (preheat_complete)
  {
    change_oven_state(OVEN_S_BAKING);
    // tell UI preheat complete
    printf("[OVEN] PREHEAT complete\n");
  }
}

void oven_baking_init()
{
  current_top_heater_state = OVEN_S_TOP_HEATER_ON;
  current_deck_heater_state = OVEN_S_DECK_HEATER_ON;
  q_enqueue(ui_queue, UI_UPDATE_BAKING_PHASE, "baking");
  printf("[OVEN] BAKING started\n");
}

void oven_baking()
{
#ifdef EMULATION
  uint32_t elapsed_s = elapsed_s_since_oven_state(OVEN_S_BAKING);
  uint32_t elapsed_m = elapsed_s / EMU_BAKING_SECONDS_PER_MINUTE;
  baking_remaining_m = baking_duration_m - elapsed_m;
#endif

  if (baking_remaining_m == 0)
  {
    change_oven_state(OVEN_S_COOLDOWN);
    current_top_heater_state = OVEN_S_TOP_HEATER_OFF;
    current_deck_heater_state = OVEN_S_DECK_HEATER_OFF;
    printf("[OVEN] BAKING complete\n");
  }

  q_enqueue(ui_queue, UI_UPDATE_REMAINING_M, &baking_remaining_m);
}

void oven_cooldown_init()
{
  current_top_heater_state = OVEN_S_TOP_HEATER_OFF;
  current_deck_heater_state = OVEN_S_DECK_HEATER_OFF;
  q_enqueue(ui_queue, UI_UPDATE_BAKING_PHASE, "cooldown");
  printf("[OVEN] COOLDOWN started\n");
}

void oven_cooldown()
{
  bool cooldown_complete = (top_heater_temperature <= HEATER_TEMPERATURE_COOL &&
                            deck_heater_temperature <= HEATER_TEMPERATURE_COOL);
  if (cooldown_complete)
  {
    change_oven_state(OVEN_S_IDLE);
    printf("[OVEN] COOLDOWN complete\n");
  }
}
