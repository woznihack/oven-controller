
#ifdef EMULATION
    #include "ArduinoFake.h"
#else
    #include "Arduino.h"
#endif

#include "controller.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../main.h"
#include "oven.h"

// HELPERS
static void sleep_ms(int ms) { usleep(1000 * ms); }
static void monitor_data_update();
static void monitor_data_print();
static void control_queue_handler();

// GLOBALS
static oven_t oven;
uint32_t last_control_queue_ms;
uint32_t last_probe_sampling_ms;
uint32_t last_state_handling_ms;
uint32_t last_data_update_ms;
uint32_t last_debug_ms;
uint32_t last_actuators_handling_ms;

void controller_init() {
  oven = oven_create();
  last_control_queue_ms = millis();
  last_probe_sampling_ms = millis();
  last_state_handling_ms = millis();
  last_data_update_ms = millis();
  last_actuators_handling_ms = millis();
  last_debug_ms = millis();
}

void controller_loop() {
  while (1) {
    uint32_t now = millis();

    if (now - last_control_queue_ms > OVEN_CONTROL_QUEUE_INTERVAL_MS) {
      control_queue_handler();
      last_control_queue_ms = now;
    }

    if (now - last_probe_sampling_ms > OVEN_PROBE_SAMPLE_INTERVAL_MS) {
      control_queue_handler();
      last_probe_sampling_ms = now;
    }

    if (now - last_state_handling_ms > OVEN_STATE_HANDLE_INTERVAL_MS){
      oven_handle_state(&oven);
      last_state_handling_ms = now;
    }

    if (now - last_data_update_ms > OVEN_DATA_UPDATE_INTERVAL_MS) {
      monitor_data_update();
      last_data_update_ms = now;
    }

    if (now - last_actuators_handling_ms > OVEN_ACTUATOR_HANDLE_INTERVAL_MS){
      oven_handle_actuators(&oven);
      last_actuators_handling_ms = now;
    }

    if (now - last_debug_ms > OVEN_DEBUG_INTERVAL_MS){
      monitor_data_print();
      last_debug_ms = now;
    }
  }
}

void monitor_data_update() {
  oven_monitor_data_t monitor_data = oven_get_monitor_data(&oven);
  q_enqueue(oven_monitor_q, MONITOR_UPDATE_OVEN_DATA, &monitor_data);
}

static time_t last_monitor_data_print;
void monitor_data_print() {
  last_monitor_data_print = time(NULL);
  oven_monitor_data_t monitor_data = oven_get_monitor_data(&oven);
  debug("[OVEN] Monitor data dump\n");
  debug("[OVEN] chamber temperature = %d °C\n", monitor_data.temperature);
}

void control_queue_handler() {
  q_node_t data;
  while (q_dequeue(oven_control_q, &data)) {
    debug("[OVEN] Handling event code = %d\n", data.event);

    // BAKING CONFIGURATION
    if (data.event == CONTROL_OVEN_SET_BAKING_PROGRAM) {
      oven.program = (baking_program_t *)data.payload;
    }

    // OVEN EVENTS CONTROLS
    if (data.event == CONTROL_OVEN_START) {
      oven_state_change(&oven, OVEN_S_PREHEATING);
    }
    if (data.event == CONTROL_OVEN_PAUSE) {
      oven_state_change(&oven, OVEN_S_BAKING_PAUSED);
    }
    if (data.event == CONTROL_OVEN_RESUME) {
      oven_state_change(&oven, OVEN_S_BAKING);
    }
    if (data.event == CONTROL_OVEN_STOP) {
      oven_state_change(&oven, OVEN_S_IDLE);
    }

    // PROGRAM OVERRIDES
    if (oven.program != NULL) {
      baking_program_step_t *curr_step = baking_program_current_step(oven.program);
      if (curr_step != NULL) {
        if (data.event == CONTROL_OVEN_SET_TEMPERATURE) {
          curr_step->temperature = *(uint32_t *)data.payload;
        }
        if (data.event == CONTROL_OVEN_SET_DURATION_M) {
          curr_step->duration_m = *(uint32_t *)data.payload;
        }
        if (data.event == CONTROL_OVEN_LIGHT_ON) {
          curr_step->light_on = true;
        }
        if (data.event == CONTROL_OVEN_LIGHT_OFF) {
          curr_step->light_on = false;
        }
        if (data.event == CONTROL_OVEN_FAN_ON) {
          curr_step->fan_on = true;
        }
        if (data.event == CONTROL_OVEN_FAN_OFF) {
          curr_step->fan_on = false;
        }
        if (data.event == CONTROL_OVEN_TOP_HEATER_ON) {
          curr_step->top_heater_on = true;
        }
        if (data.event == CONTROL_OVEN_TOP_HEATER_OFF) {
          curr_step->top_heater_on = false;
        }
        if (data.event == CONTROL_OVEN_DECK_HEATER_ON) {
          curr_step->deck_heater_on = true;
        }
        if (data.event == CONTROL_OVEN_DECK_HEATER_OFF) {
          curr_step->deck_heater_on = false;
        }
        if (data.event == CONTROL_OVEN_GRILL_ON) {
          curr_step->grill_on = true;
        }
        if (data.event == CONTROL_OVEN_GRILL_OFF) {
          curr_step->grill_on = false;
        }
      }
    }
  }
}
