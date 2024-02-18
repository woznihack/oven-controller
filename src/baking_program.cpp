#include "baking_program.h"

#include <main.h>
#include <stdlib.h>

void baking_program_init_step(baking_program_step_t *s) {
  s->top_heater_temperature = BAKING_TEMPERATURE_MIN;
  s->deck_heater_temperature = BAKING_TEMPERATURE_MIN;
  s->duration_m = BAKING_DURATION_M_MIN;
  s->light_on = true;
  s->fan_on = false;
  s->deck_heater_on = true;
  s->top_heater_on = true;
  s->grill_on = false;
}

baking_program_t baking_program_create(char *program_name) {
  baking_program_t p;
  p.steps_count = 0;
  p.program_name = program_name;
  p.steps = NULL;
  return p;
}

void baking_program_reset(baking_program_t *p) {
  free(p->steps);
  p->steps_count = 0;
}

void baking_program_add_step(baking_program_t *p) {
  if (p->steps_count == BAKING_MAX_STEPS) return;

  baking_program_step_t *new_ptr;

  if (p->steps == NULL) {
    new_ptr = (baking_program_step_t *)malloc(sizeof(baking_program_step_t));
  } else {
    new_ptr = (baking_program_step_t *)realloc(p->steps, sizeof(baking_program_step_t) * p->steps_count);
  }


  if (new_ptr == NULL){
    debug("[BAKING_PROGRAM] Error allocating memory !");
    return;
  }

  p->steps = new_ptr;
  baking_program_init_step(&p->steps[p->steps_count]);
  p->steps_count++;
}

void baking_program_delete_last_step(baking_program_t *p) {
  if (p->steps_count == 0) return;
  p->steps_count--;
  baking_program_step_t *new_ptr = (baking_program_step_t *)realloc(p->steps, sizeof(baking_program_step_t) * p->steps_count);

  if (new_ptr == NULL){
    debug("[BAKING_PROGRAM] Error allocating memory !");
    return;
  }
  p->steps = new_ptr;
}
