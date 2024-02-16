// #ifndef BAKING_PROGRAM_H
// #define BAKING_PROGRAM_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct {
  bool light_on;
  bool fan_on;
  bool top_heater_on;
  bool deck_heater_on;
  bool grill_on;

  uint32_t top_heater_temperature;
  uint32_t deck_heater_temperature;
  uint32_t duration_m;
} baking_program_step_t;

typedef struct {
  char *program_name;
  uint16_t steps_count;
  baking_program_step_t *steps;
} baking_program_t;

baking_program_step_t baking_program_step_create();
baking_program_t baking_program_create(char *);
void baking_program_add_step(baking_program_t *, baking_program_step_t);
void baking_program_delete_last_step(baking_program_t *);
void baking_program_reset(baking_program_t *);

// #endif
