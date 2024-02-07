#include <unistd.h>
#include <stdio.h>

#include "controller.h"
#include "heater.h"

heater_t heater_create()
{
  heater_t h;
  h.current_state = HEATER_S_OFF,
  h.target_temperature = EMU_HEATER_ENV_TEMPERATURE;
  h.current_temperature = EMU_HEATER_ENV_TEMPERATURE;
  return h;
}

void heater_loop(heater_t *h)
{
  (heater_sm[get_heater_state(h)].stateFunc)(h);
}

bool is_heater_state(heater_t *h, heater_state_t s)
{
  return h->current_state == s;
}

void change_heater_state(heater_t *h, heater_state_t s)
{
  h->current_state = s;
  h->state_start_s[s] = time(NULL);
  (heater_sm[h->current_state].initStateFunc)(h);
}

heater_state_t get_heater_state(heater_t *h)
{
  return h->current_state;
}

uint32_t elapsed_s_since_heater_state(heater_t *h, heater_state_t s)
{
  return difftime(time(NULL), h->state_start_s[s]);
}

void heater_on_init(heater_t *h) {}
void heater_on(heater_t *h)
{

#ifdef EMULATION
  if (h->current_temperature < h->target_temperature)
  {
    double elapsed_s = elapsed_s_since_heater_state(h, HEATER_S_ON);
    uint32_t delta_temperature = h->target_temperature - EMU_HEATER_ENV_TEMPERATURE;
    h->current_temperature = EMU_HEATER_ENV_TEMPERATURE + delta_temperature * EMU_HEATER_PER_SECOND_TEMP_INCREASE_PCT * elapsed_s;
    if (h->current_temperature > h->target_temperature)
    {
      h->current_temperature = h->target_temperature;
    }
  }
#endif
}

void heater_off_init(heater_t *h) {}
void heater_off(heater_t *h)
{
#ifdef EMULATION
  if (h->current_temperature > EMU_HEATER_ENV_TEMPERATURE)
  {
    double elapsed_s = elapsed_s_since_heater_state(h, HEATER_S_OFF);
    uint32_t delta_temperature = h->target_temperature - EMU_HEATER_ENV_TEMPERATURE;
    h->current_temperature = h->current_temperature - delta_temperature * EMU_HEATER_PER_SECOND_TEMP_DECREASE_PCT * elapsed_s;
    if (h->current_temperature < EMU_HEATER_ENV_TEMPERATURE)
    {
      h->current_temperature = EMU_HEATER_ENV_TEMPERATURE;
    }
  }
#endif
}
