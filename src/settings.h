#pragma once

#if defined(_WIN64) || defined(_WIN32) || defined(__APPLE__) || defined(__linux__)

// EMULATION SETTINGS
#define EMULATION 1
#define EMU_BAKING_SECONDS_PER_MINUTE 5
#define EMU_TEMP_INC_PER_ON_CYCLE 0.5  // increase of temperature for each time the heaters are on
#define EMU_TEMP_DECAY_PER_SECOND 0.5  // decrease/s of T for each loop of actuator handling
#define EMU_TEMP_INIT 20               // initial chamber temperature
#endif

// SETTINGS
#define DEBUG 1

#define OVEN_TEMP_COOL 35                       // temperature at which consider oven cool
#define OVEN_CONTROL_QUEUE_INTERVAL_MS 200      // interval for reading oven control data from UI
#define OVEN_MONITOR_QUEUE_INTERVAL_MS 200      // interval for sending oven data to UI
#define OVEN_STATE_HANDLING_INTERVAL_MS 200     // interval to handle the oven state
#define OVEN_PROBE_SAMPLING_INTERVAL_MS 200     // interval to read the probes
#define OVEN_ACTUATOR_HANDLING_INTERVAL_MS 100  // interval to act on the actuators

#define OVEN_DATA_DUMP 1
#define OVEN_DATA_DUMP_INTERVAL_MS 5000         // interval to print oven debug messages

#define OVEN_CONTROL_PID_SAMPLE_TIME_MS 1000 // interval to recalculate the PID samples
#define OVEN_CONTROL_WINDOW_SIZE_MS 2000     // heaters control window size

#define BAKING_MAX_STEPS 3
#define BAKING_TEMPERATURE_MIN 30
#define BAKING_TEMPERATURE_MAX 300
#define BAKING_DURATION_M_MIN 1
#define BAKING_DURATION_M_MAX 60 * 10

#define FAN_PIN 1
#define LIGHT_PIN 2
#define TOP_HEATER_PIN 3
#define DECK_HEATER_PIN 4
#define CHAMBER_PROBE_PIN 5
