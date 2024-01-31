#include <stdlib.h>
#include "controller.h"

program_t * create_oven_program(char * _name, int _setTemperature, int _setDurationMinutes){
  program_t * p = (program_t *)malloc(sizeof(program_t));
  p->name = _name;
  p->started = false;
  p->startedAt = 0;
  p->setTemperature = _setTemperature;
  p->setDurationMinutes = _setDurationMinutes;
  p->currentTemperature = 0;
  p->remainingMinutes = 0;
  return p;
}
