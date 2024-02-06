#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "controller.h"
#include "oven_state.h"

// HELPERS
void sleep_ms(int ms) { usleep(1000 * ms); }
void print_info();
void send_monitor_data();

// PROTOTYPES
void controller_init();
void controller_loop();
void handle_queue_messages();
void read_probes();

// GLOBALS
oven_light_state_t current_light_state = OVEN_S_LIGHT_OFF;
oven_fan_state_t current_fan_state = OVEN_S_FAN_OFF;
oven_top_heater_state_t current_top_heater_state = OVEN_S_TOP_HEATER_OFF;
oven_deck_heater_state_t current_deck_heater_state = OVEN_S_DECK_HEATER_OFF;

uint32_t top_heater_temperature = 0;
uint32_t deck_heater_temperature = 0;
uint32_t baking_duration_m = 0;
uint32_t baking_temperature = 0;
uint32_t baking_remaining_m = 0;

time_t controller_start_s;

// for debugging
const char *light_state_strings[] = {"ON", "OFF"};
const char *fan_state_strings[] = {"ON", "OFF"};
const char *top_heater_state_strings[] = {"ON", "OFF"};
const char *deck_heater_state_string[] = {"ON", "OFF"};

void controller_init() {}

void controller_loop()
{
  controller_start_s = time(NULL);

  while (1)
  {
    handle_queue_messages();
    read_probes();

    (oven_sm[get_oven_state()].stateFunc)();
    (fan_sm[current_fan_state].stateFunc)();
    (light_sm[current_light_state].stateFunc)();
    (top_heater_sm[current_top_heater_state].stateFunc)();
    (deck_heater_sm[current_deck_heater_state].stateFunc)();

    print_info();
    send_monitor_data();
    sleep_ms(1000);
  }
}

void light_on() {}
void light_off() {}
void fan_on() {}
void fan_off() {}
void top_heater_on()
{
#ifdef EMULATION
  if (is_oven_state(OVEN_S_PREHEATING))
  {
    double elapsed_s = elapsed_s_since_oven_state(OVEN_S_PREHEATING);
    top_heater_temperature = floor((double)baking_temperature * EMU_PREHEAT_PER_SECOND_INCREASE_PCT) * elapsed_s;
  }
#endif
}

void top_heater_off()
{
#ifdef EMULATION
  if (is_oven_state(OVEN_S_COOLDOWN))
  {
    double elapsed_s = elapsed_s_since_oven_state(OVEN_S_COOLDOWN);
    top_heater_temperature = baking_temperature - floor((double)baking_temperature * EMU_COOLDOWN_PER_SECOND_DECREASE_PCT) * elapsed_s;
  }
#endif
}

void deck_heater_on()
{
  printf("[OVEN] bottom heater on\n");

#ifdef EMULATION
  if (is_oven_state(OVEN_S_PREHEATING))
  {
    double elapsed_s = elapsed_s_since_oven_state(OVEN_S_PREHEATING);
    deck_heater_temperature = floor((double)baking_temperature * EMU_PREHEAT_PER_SECOND_INCREASE_PCT) * elapsed_s;
  }
#endif
}

void deck_heater_off()
{
#ifdef EMULATION
  if (is_oven_state(OVEN_S_COOLDOWN))
  {
    double elapsed_s = elapsed_s_since_oven_state(OVEN_S_COOLDOWN);
    printf("[OVEN] cooldown bottom heater OFF handle\n");
    printf("[OVEN] elapsed s = %.2f\n", elapsed_s);
    printf("[OVEN] decrease temp of= %.2f\n", floor((double)baking_temperature * EMU_COOLDOWN_PER_SECOND_DECREASE_PCT) * elapsed_s);
    deck_heater_temperature = baking_temperature - floor((double)baking_temperature * EMU_COOLDOWN_PER_SECOND_DECREASE_PCT) * elapsed_s;
  }
#endif
}

void read_probes() {}
void print_info()
{
  int diff_seconds = (int)difftime(time(NULL), controller_start_s);
  if (diff_seconds % 10 == 0)
  {
    printf("[OVEN] State dump\n");
    printf("[OVEN] [oven_state = %s, fan = %s, light = %s, top_heater = %s, deck_heater = %s ]\n",
           oven_state_strings[get_oven_state()],
           fan_state_strings[current_fan_state],
           light_state_strings[current_light_state],
           top_heater_state_strings[current_top_heater_state],
           deck_heater_state_string[current_deck_heater_state]);
    printf("[OVEN] [top_t = %dC, deck_t = %dC, remaining_m = %d of %d]\n",
           top_heater_temperature,
           deck_heater_temperature,
           baking_remaining_m,
           baking_duration_m);
  }
}

void send_monitor_data()
{
  if (
      is_oven_state(OVEN_S_PREHEATING) ||
      is_oven_state(OVEN_S_BAKING) ||
      is_oven_state(OVEN_S_COOLDOWN))
  {
    q_enqueue(ui_queue, UI_UPDATE_REMAINING_M, &baking_remaining_m);
    q_enqueue(ui_queue, UI_UPDATE_TOP_HEATER_TEMP, &top_heater_temperature);
    q_enqueue(ui_queue, UI_UPDATE_DECK_HEATER_TEMP, &deck_heater_temperature);
  }
}

void handle_queue_messages()
{
  q_node_t data;
  while (q_dequeue(oven_queue, &data))
  {
    printf("[OVEN] Handling event code = %d\n", data.event);
    if (data.event == OVEN_START)
    {
      change_oven_state(OVEN_S_PREHEATING);
    }
    if (data.event == OVEN_STOP)
    {
      change_oven_state(OVEN_S_IDLE);
    }
    if (data.event == OVEN_SET_TEMPERATURE)
    {
      baking_temperature = *(int *)data.payload;
    }
    if (data.event == OVEN_SET_DURATION_M)
    {
      baking_duration_m = *(int *)data.payload;
      baking_remaining_m = baking_duration_m;
    }
  }
}
