#ifndef DS_H
#define DS_H

#include "../shared_data_structures.h"
#include "../settings.h"

typedef struct {
  bool ready;
  oven_data_t data;
} oven_monitor_t;

typedef struct
{
  char *program_name;
  uint16_t last_step_idx;
  uint16_t curr_step_idx;
  oven_data_t steps[BAKING_MAX_STEPS];
  bool paused;
} program_t;

static oven_monitor_t oven_monitor_create(){
  oven_monitor_t m;
  m.ready = false;
  return m;
}

static oven_data_t program_step_create(){
  oven_data_t s;
  s.temperature_set = BAKING_TEMPERATURE_MIN;
  s.duration_m_set = BAKING_DURATION_M_MIN;
  s.light_on = true;
  s.fan_on = false;
  s.deck_heater_on = true;
  s.top_heater_on = true;
  s.grill_on = false;
  return s;
}

static program_t program_create(char *program_name){
  program_t p;
  p.last_step_idx = 0;
  p.curr_step_idx = 0;
  p.steps[0] = program_step_create();
  p.program_name = program_name;
  p.paused = false;
  return p;
}

static void program_add_step(program_t *p){
  if (p->last_step_idx + 1 >= BAKING_MAX_STEPS)
    return;

  uint16_t step_idx = p->last_step_idx + 1;
  p->steps[step_idx] = program_step_create();
  p->last_step_idx = step_idx;
}

static void program_delete_last_step(program_t *p){
  if (p->last_step_idx == 0)
    return;

  p->last_step_idx--;
}

#endif
