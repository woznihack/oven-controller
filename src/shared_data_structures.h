#ifndef SDS_H
#define SDS_H

// this same data structure is used bot to
// configure the program steps and to
// monitor the current oven data
typedef struct  {

  // config: set/unset function, monitor: check current function state
  bool light_on;
  bool fan_on;
  bool top_heater_on;
  bool deck_heater_on;
  bool grill_on;

  uint32_t temperature; // config: set temperature, monitor: current temperature
  uint32_t duration_m; // config: set duration minutes
  uint32_t remaining_m; // monitor: current remaining minutes

  // monitor only parameters
  char *current_phase;
  uint16_t current_step;
  uint16_t total_steps;

} oven_data_t;

#endif
