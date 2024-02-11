#include "controller.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../main.h"
#include "heater.h"
#include "oven.h"

// HELPERS
static void sleep_ms(int ms) { usleep(1000 * ms); }
static void monitor_data_update();
static void monitor_data_print();
static void control_queue_handler();
static void probes_read();

// GLOBALS
static oven_light_state_t current_light_state = OVEN_S_LIGHT_OFF;
static oven_fan_state_t current_fan_state = OVEN_S_FAN_OFF;

static time_t controller_start_s;
static oven_t oven;

void controller_init() { oven = oven_create(); }

void controller_loop() {
  controller_start_s = time(NULL);

  while (1) {
    control_queue_handler();
    probes_read();

    // state machine loop
    oven_loop(&oven);
    (fan_sm[current_fan_state].stateFunc)();
    (light_sm[current_light_state].stateFunc)();

    monitor_data_update();
    monitor_data_print();

    sleep_ms(1000);
  }
}

void probes_read() {}

void monitor_data_print() {
  int diff_seconds = (int)difftime(time(NULL), controller_start_s);
  if (diff_seconds % 10 == 0) {
    debug("[OVEN] State dump\n");
    debug("[OVEN] [oven_state = %s, fan = %s, light = %s, top_heater = %s, deck_heater = %s ]\n", oven_state_strings[oven_get_state(&oven)], fan_state_strings[current_fan_state], light_state_strings[current_light_state], top_heater_state_strings[get_heater_state(&oven.top_heater)], deck_heater_state_string[get_heater_state(&oven.deck_heater)]);
    debug("[OVEN] [top_t = %dC, deck_t = %dC, remaining_m = %d of %d]\n", oven.top_heater.current_temperature, oven.deck_heater.current_temperature, oven.data.remaining_m, oven.data.duration_m_set);
  }
}

void monitor_data_update() { q_enqueue(oven_monitor_q, MONITOR_UPDATE_OVEN_DATA, &oven.data); }

void control_queue_handler() {
  q_node_t data;
  while (q_dequeue(oven_control_q, &data)) {
    debug("[OVEN] Handling event code = %d\n", data.event);
    if (data.event == CONTROL_OVEN_SET_BAKING_STEPS_COUNT) {
      oven.baking_steps_count = *(uint16_t *)data.payload;
      debug("[OVEN] received baking steps count %d\n", oven.baking_steps_count);
    }
    if (data.event == CONTROL_OVEN_SET_BAKING_STEPS) {
      oven.baking_steps = (oven_data_t *)data.payload;
      debug("[OVEN] received baking steps\n");
    }
    if (data.event == CONTROL_OVEN_START) {
      oven_change_state(&oven, OVEN_S_PREHEATING);
    }
    if (data.event == CONTROL_OVEN_STOP) {
      oven_change_state(&oven, OVEN_S_IDLE);
    }
  }
}

void light_on() {}
void light_off() {}
void fan_on() {}
void fan_off() {}
