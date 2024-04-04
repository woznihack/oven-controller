#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../queue.h"

extern q_queue_t *oven_control_q;
extern q_queue_t *oven_monitor_q;

// controller
void controller_init();
void controller_loop();

#endif  // CONTROLLER_H
