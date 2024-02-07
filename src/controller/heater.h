#ifndef HEATER_STATE_H
#define HEATER_STATE_H

#include "../settings.h"

typedef enum
{
  HEATER_S_ON = 0,
  HEATER_S_OFF
} heater_state_t;

typedef struct
{
  uint32_t target_temperature;
  uint32_t current_temperature;
  heater_state_t current_state;
  time_t state_start_s[4];
} heater_t;

typedef struct
{
  heater_state_t state;
  void (*stateFunc)(heater_t *);
  void (*initStateFunc)(heater_t *);
} heater_state_node_t;


// heater state init functions
void heater_on_init(heater_t *);
void heater_off_init(heater_t *);

// heater state functions
void heater_on(heater_t *);
void heater_off(heater_t *);

// heater functions
bool is_heater_state(heater_t *, heater_state_t);
void change_heater_state(heater_t *, heater_state_t);
heater_state_t get_heater_state(heater_t *);
uint32_t elapsed_s_since_heater_state(heater_t *, heater_state_t);
heater_t heater_create();
void heater_loop(heater_t *h);

static heater_state_node_t heater_sm[] = {
    // light state machine
    {HEATER_S_ON, heater_on, heater_on_init},
    {HEATER_S_OFF, heater_off, heater_off_init},
};

#endif
