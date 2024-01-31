#ifndef CONTROLLER_H
#define CONTROLLER_H

typedef struct {
  char * name;
  bool started;
  unsigned long startedAt;
  int setTemperature;
  int setDurationMinutes;
  int currentTemperature;
  int remainingMinutes;
} program_t;

program_t * create_oven_program(char * _name, int _setTemperature, int _setDurationMinutes);

#endif // CONTROLLER_H
