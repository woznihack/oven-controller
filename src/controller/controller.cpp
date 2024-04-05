
#include "Arduino.h"

#include "controller.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../main.h"
#include "oven.h"

// GLOBALS
static oven_t oven;
// timers counting the milliseconds for the control functions
uint32_t last_control_queue_ms;
uint32_t last_monitor_queue_ms;
uint32_t last_probe_sampling_ms;
uint32_t last_state_handling_ms;
uint32_t last_actuators_handling_ms;
uint32_t last_data_dump_ms;

void controller_init() {
  oven = oven_create();
  oven_init_controllers(&oven);
  debug("[CONTROLLER] Address if chamber_probe_reading %p\n", &oven.chamber_probe_reading);
  last_control_queue_ms = millis();
  last_monitor_queue_ms = millis();
  last_probe_sampling_ms = millis();
  last_state_handling_ms = millis();
  last_actuators_handling_ms = millis();
  last_data_dump_ms = millis();
}

void controller_loop() {
  while (1) {
    uint32_t now = millis();

    if (now - last_control_queue_ms > OVEN_CONTROL_QUEUE_INTERVAL_MS) {
      q_node_t data;
      while (q_dequeue(oven_control_q, &data)) {
        oven_handle_control_message(&oven, data);
      }
      last_control_queue_ms = now;
    }

    if (now - last_monitor_queue_ms > OVEN_MONITOR_QUEUE_INTERVAL_MS) {
      oven_monitor_data_t monitor_data = oven_get_monitor_data(&oven);
      q_enqueue(oven_monitor_q, MONITOR_UPDATE_OVEN_DATA, &monitor_data);
      last_monitor_queue_ms = now;
    }

    if (now - last_probe_sampling_ms > OVEN_PROBE_SAMPLING_INTERVAL_MS) {
      oven_read_probes(&oven);
      last_probe_sampling_ms = now;
    }

    if (now - last_state_handling_ms > OVEN_STATE_HANDLING_INTERVAL_MS){
      oven_handle_state(&oven);
      last_state_handling_ms = now;
    }


    if (now - last_actuators_handling_ms > OVEN_ACTUATOR_HANDLING_INTERVAL_MS){
      oven_handle_actuators(&oven);
      last_actuators_handling_ms = now;
    }

#if OVEN_DATA_DUMP
    if (now - last_data_dump_ms > OVEN_DATA_DUMP_INTERVAL_MS){
      oven_monitor_data_t monitor_data = oven_get_monitor_data(&oven);
      debug("[OVEN] Monitor data dump\n");
      debug("[OVEN] chamber temperature = %.2f °C\n", monitor_data.temperature);
      last_data_dump_ms = now;
    }
#endif
  }
}
