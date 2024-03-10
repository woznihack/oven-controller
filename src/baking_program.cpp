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
  p.program_name = program_name;
  p.steps_count = 0;
  p.steps = (baking_program_step_t *)malloc(BAKING_MAX_STEPS * sizeof(baking_program_step_t));
  baking_program_add_step(&p);
  return p;
}

void baking_program_reset(baking_program_t *p) {
  p->steps_count = 0;
  baking_program_add_step(p);
}

void baking_program_add_step(baking_program_t *p) {
  if (p->steps_count == BAKING_MAX_STEPS) return;
  baking_program_init_step(&p->steps[p->steps_count]);
  p->steps_count++;
}

void baking_program_delete_step(baking_program_t *p, uint16_t step_idx){
  if (p->steps_count == 1 || step_idx < 0 && step_idx >= p->steps_count){
    debug("[BAKING_PROGRAM] idx out of bounds %d (%d, %d)\n", step_idx, 0, p->steps_count);
    return;
  }

  for(uint16_t s = step_idx; s < p->steps_count - 1; s++){
    debug("[BAKING_PROGRAM] copying %d in %d\n", s+1, s);
    p->steps[s] = p->steps[s+1];
  }
  p->steps_count--;
}
