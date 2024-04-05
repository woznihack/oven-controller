#include "baking_program.h"

#include <main.h>
#include <math.h>

// static functions
static void baking_program_init_step(baking_program_step_t *s);

void baking_program_init_step(baking_program_step_t *s) {
  s->temperature = BAKING_TEMPERATURE_MIN;
  s->duration_m = BAKING_DURATION_M_MIN;
  s->light_on = true;
  s->fan_on = false;
  s->deck_heater_on = true;
  s->top_heater_on = true;
}

baking_program_t baking_program_create(char *program_name) {
  baking_program_t p;
  p.program_name = program_name;
  p.steps_count = 0;
  p.steps_count = 0;
  p.current_step_idx = 1;
  p.started = false;
  p.paused = false;
  p.current_step_idx = UINT16_MAX;
  p.steps = (baking_program_step_t *)malloc(BAKING_MAX_STEPS * sizeof(baking_program_step_t));
  baking_program_add_step(&p);
  return p;
}

void baking_program_steps_reset(baking_program_t *p) {
  p->steps_count = 0;
  baking_program_add_step(p);
}

bool baking_program_add_step(baking_program_t *p) {
  if (p->steps_count == BAKING_MAX_STEPS) return false;
  baking_program_init_step(&p->steps[p->steps_count]);
  p->steps_count++;
  return true;
}

void baking_program_delete_step(baking_program_t *p, uint16_t step_idx) {
  if (p->steps_count == 1 || (step_idx < 0 && step_idx >= p->steps_count)) {
    debug("[BAKING_PROGRAM] idx out of bounds %d (%d, %d)\n", step_idx, 0, p->steps_count);
    return;
  }

  for (uint16_t s = step_idx; s < p->steps_count - 1; s++) {
    debug("[BAKING_PROGRAM] copying %d in %d\n", s + 1, s);
    p->steps[s] = p->steps[s + 1];
  }
  p->steps_count--;
}
void baking_program_start(baking_program_t *p) {
  if (p->started) {
    return;
  }

  p->current_step_idx = UINT16_MAX;
  p->started = true;
  baking_program_goto_next_step(p);
}

void baking_program_pause(baking_program_t *p) {
  if (!p->started || p->paused) {
    return;
  }

  p->paused = true;
  p->paused_at = time(NULL);
  debug("[PROGRAM][RESUME] pausing program\n");
}

void baking_program_resume(baking_program_t *p) {
  if (!p->started || !p->paused) {
    return;
  }

  p->paused = false;
  // increase the step start time of the pause duration
  // in order to resume the current step from where it was left
  p->current_step_started_at += difftime(time(NULL), p->paused_at);
  debug("[PROGRAM][RESUME] resuming program\n");
}

void baking_program_reset(baking_program_t *p) {
  p->current_step_idx = UINT16_MAX;
  p->started = false;
}

bool baking_program_goto_next_step(baking_program_t *p) {
  uint16_t next_step_idx = p->current_step_idx == UINT16_MAX ? 0 : (p->current_step_idx + 1);
  if (next_step_idx >= p->steps_count) {
    return false;
  }
  p->current_step_started_at = time(NULL);
  p->current_step_idx = next_step_idx;

  debug("[PROGRAM] goto next step (%d/%d)\n", next_step_idx + 1, p->steps_count);
  return true;
}

double baking_program_current_step_remaining_s(baking_program_t *p) {
  double elapsed_s = difftime(time(NULL), p->current_step_started_at);
  double remaining_s;
#ifdef EMULATION
  remaining_s = p->steps[p->current_step_idx].duration_m * EMU_BAKING_SECONDS_PER_MINUTE - elapsed_s;
#else
  remaining_s = p->steps[p->current_step_idx].duration_m * 60 - elapsed_s;
#endif
  return remaining_s;
}

uint16_t baking_program_current_step_remaining_m(baking_program_t *p) {
  double remaining_s = baking_program_current_step_remaining_s(p);

#ifdef EMULATION
  double remaining_m = ceil(remaining_s / EMU_BAKING_SECONDS_PER_MINUTE);
#else
  double remaining_m = ceil(remaining_s / 60);
#endif

  uint16_t remaining_m_return = (uint16_t)remaining_m;
  //debug("[PROGRAM][CURR_STEP_REMAINING_M] seconds: %.2f, minutes: %.2f, minutes return %d\n", remaining_s, remaining_m, remaining_m_return);
  return remaining_m_return;
}

bool baking_program_is_complete(baking_program_t *p) {
  bool current_step_is_complete = baking_program_current_step_is_complete(p);
  bool current_step_is_last = p->current_step_idx == p->steps_count - 1;
  return current_step_is_complete && current_step_is_last;
}

bool baking_program_current_step_is_complete(baking_program_t *p) {
  double remaining_s = baking_program_current_step_remaining_s(p);
  return remaining_s <= 0.0;
}

baking_program_step_t *baking_program_get_step(baking_program_t *p, uint16_t step_idx) {
  if (step_idx < 0 || step_idx >= p->steps_count) {
    return NULL;
  }

  return &p->steps[step_idx];
}
baking_program_step_t *baking_program_get_current_step(baking_program_t *p) {
  if (p->current_step_idx == UINT16_MAX || p->current_step_idx >= p->steps_count) {
    return NULL;
  }

  return &p->steps[p->current_step_idx];
}
