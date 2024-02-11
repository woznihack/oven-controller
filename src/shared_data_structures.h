#ifndef SDS_H
#define SDS_H

typedef struct  {
  uint32_t temperature;
  uint32_t duration_m;
  uint32_t remaining_m;
  bool light_on;
  bool fan_on;
  bool top_heater_on;
  bool deck_heater_on;
  bool grill_on;
  uint16_t current_step;
  uint16_t total_steps;
  char *current_phase;
} oven_data_t;

#endif
