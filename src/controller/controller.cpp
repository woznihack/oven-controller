#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "controller.h"
#include "oven.h"
#include "heater.h"

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

time_t controller_start_s;

oven_t oven;
oven_data_t oven_data;
oven_data_t *baking_steps;
uint16_t baking_steps_count;

void controller_init() {
  oven = oven_create();
}

void controller_loop()
{
  controller_start_s = time(NULL);

  while (1)
  {
    handle_queue_messages();
    read_probes();

    // state machine loop
    oven_loop(&oven);
    (fan_sm[current_fan_state].stateFunc)();
    (light_sm[current_light_state].stateFunc)();

    print_info();
    send_monitor_data();
    sleep_ms(1000);
  }
}

void light_on() {}
void light_off() {}
void fan_on() {}
void fan_off() {}

void read_probes() {}
void print_info()
{
  int diff_seconds = (int)difftime(time(NULL), controller_start_s);
  if (diff_seconds % 10 == 0)
  {
    printf("[OVEN] State dump\n");
    printf("[OVEN] [oven_state = %s, fan = %s, light = %s, top_heater = %s, deck_heater = %s ]\n",
           oven_state_strings[get_oven_state(&oven)],
           fan_state_strings[current_fan_state],
           light_state_strings[current_light_state],
           top_heater_state_strings[get_heater_state(&oven.top_heater)],
           deck_heater_state_string[get_heater_state(&oven.deck_heater)]);
    printf("[OVEN] [top_t = %dC, deck_t = %dC, remaining_m = %d of %d]\n",
           oven.top_heater.current_temperature,
           oven.deck_heater.current_temperature,
           oven.data.remaining_m,
           oven.data.duration_m);
  }
}

void send_monitor_data()
{
  if (
      is_oven_state(&oven, OVEN_S_PREHEATING) ||
      is_oven_state(&oven, OVEN_S_BAKING) ||
      is_oven_state(&oven, OVEN_S_COOLDOWN))
  {
    q_enqueue(ui_queue, UI_UPDATE_OVEN_DATA, &oven.data);
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
      printf("[OVEN] Handling event OVEN_START\n");
      change_oven_state(&oven, OVEN_S_PREHEATING);
      printf("[OVEN] Handled event OVEN_START\n");
    }
    if (data.event == OVEN_STOP)
    {
      change_oven_state(&oven, OVEN_S_IDLE);
    }
    if (data.event == OVEN_SET_BAKING_STEPS_COUNT)
    {
      baking_steps_count = *(uint16_t *)data.payload;
      printf("[OVEN] received baking steps count %d\n", baking_steps_count);
    }
    if (data.event == OVEN_SET_BAKING_STEPS)
    {
      baking_steps = (oven_data_t *)data.payload;
      printf("[OVEN] received baking steps\n");
    }
  }
}
