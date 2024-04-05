#include <Arduino.h>
#include <lv_conf.h>
#include <lvgl.h>
#include <lvgl_hal.h>
#include <runner.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "baking_program.h"
#include "controller/controller.h"
#include "controller/oven.h"
#include "settings.h"
#include "ui/ui.h"

#if EMULATION

using namespace fakeit;
#include <pthread.h>

#include <chrono>
#include <iostream>
pthread_t controller_thread;
void *controller_thread_function(void *ptr) { controller_loop(); }
#endif

q_queue_t *oven_control_q;
q_queue_t *oven_monitor_q;

MAIN() {
#if EMULATION

  When(Method(ArduinoFake(), millis)).AlwaysDo([]() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
  });
#endif

  oven_control_q = q_create(sizeof(baking_program_t) + BAKING_MAX_STEPS * sizeof(baking_program_step_t) * 2);
  oven_monitor_q = q_create(sizeof(oven_monitor_data_t));

  /*Initialize LVGL*/
  lv_init();
  controller_init();

  /*Initialize the HAL (display, input devices, tick) for LVGL*/
  hal_setup();
  ui_init();

#if EMULATION
  pthread_create(&controller_thread, NULL, controller_thread_function, NULL);
#endif

  hal_loop();  // can only be called by main thread

#if EMULATION
  pthread_join(controller_thread, NULL);
#endif

  return 0;
}
