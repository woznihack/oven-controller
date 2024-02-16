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
    // (fan_sm[current_fan_state].stateFunc)();
    // (light_sm[current_light_state].stateFunc)();

    monitor_data_update();
    monitor_data_print();

    sleep_ms(200);
  }
}

void probes_read() {}

static time_t last_monitor_data_print;
void monitor_data_print() {
  if ((int)difftime(time(NULL), last_monitor_data_print) > 10) {
    last_monitor_data_print = time(NULL);
    oven_monitor_data_t monitor_data = oven_get_monitor_data(oven);
    debug("[OVEN] Monior data dump\n");
    debug("[OVEN] top heater t = %d °C\n", monitor_data.top_heater_temperature);
    debug("[OVEN] deck heater t = %d °C\n", monitor_data.deck_heater_temperature);
  }
}

void monitor_data_update() {
  oven_monitor_data_t monitor_data = oven_get_monitor_data(oven);
  q_enqueue(oven_monitor_q, MONITOR_UPDATE_OVEN_DATA, &monitor_data);
}

void control_queue_handler() {
  q_node_t data;
  while (q_dequeue(oven_control_q, &data)) {
    debug("[OVEN] Handling event code = %d\n", data.event);

    // BAKING CONFIGURATION
    if (data.event == CONTROL_OVEN_SET_BAKING_PROGRAM) {
      debug("[OVEN] received baking program\n");
      oven.program = (baking_program_t *)data.payload;
    }

    // BAKING CONTROLS
    if (data.event == CONTROL_OVEN_START) {
      oven_change_state(&oven, OVEN_S_PREHEATING);
    }
    if (data.event == CONTROL_OVEN_STOP) {
      oven_change_state(&oven, OVEN_S_IDLE);
    }
    if (data.event == CONTROL_OVEN_SET_TOP_HEATER_TEMP) {
      oven.program->steps[oven.current_program_step].top_heater_temperature = *(uint32_t *)data.payload;
    }
    if (data.event == CONTROL_OVEN_SET_DECK_HEATER_TEMP) {
      oven.program->steps[oven.current_program_step].deck_heater_temperature = *(uint32_t *)data.payload;
    }
    if (data.event == CONTROL_OVEN_SET_DURATION_M) {
      oven.program->steps[oven.current_program_step].duration_m = *(uint32_t *)data.payload;
    }
  }
}

void light_on() {}
void light_off() {}
void fan_on() {}
void fan_off() {}
