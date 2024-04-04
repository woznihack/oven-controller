#ifndef BAKING_PROGRAM_H
#define BAKING_PROGRAM_H

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
  bool light_on;
  bool fan_on;
  bool top_heater_on;
  bool deck_heater_on;
  bool grill_on;

  uint32_t temperature;
  uint32_t duration_m;
} baking_program_step_t;

typedef struct {
  char *program_name;
  uint16_t steps_count;
  baking_program_step_t *steps;
  bool started;
  bool paused;
  time_t paused_at;
  uint16_t current_step_idx;
  time_t current_step_started_at;
} baking_program_t;

baking_program_t baking_program_create(char *);
bool baking_program_add_step(baking_program_t *);
void baking_program_delete_step(baking_program_t *, uint16_t step_idx);
void baking_program_reset(baking_program_t *);

void baking_program_start(baking_program_t *);
void baking_program_pause(baking_program_t *);
void baking_program_resume(baking_program_t *);
bool baking_program_is_complete(baking_program_t *);
bool baking_program_current_step_is_complete(baking_program_t *);
uint16_t baking_program_current_step_remaining_m(baking_program_t *);
bool baking_program_goto_next_step(baking_program_t *);
baking_program_step_t * baking_program_current_step(baking_program_t *);

#endif
