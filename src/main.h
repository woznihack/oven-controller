#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>

#include "settings.h"

#ifndef DEBUG
#define DEBUG 0
#endif

#define debug(fmt, ...)                             \
  do {                                              \
    if (DEBUG) fprintf(stderr, fmt, ##__VA_ARGS__); \
  } while (0)

#endif
