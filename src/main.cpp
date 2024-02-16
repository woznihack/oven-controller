#include <runner.h>
#include <pthread.h>
#include <lv_conf.h>
#include <lvgl_hal.h>
#include <stdio.h>
#include <lvgl.h>
#include <unistd.h>
#include <time.h>

#include "baking_program.h"
#include "ui/ui.h"
#include "controller/controller.h"

pthread_t controller_thread;
void *controller_thread_function(void *ptr)
{
    controller_loop();
}

q_queue_t *oven_control_q;
q_queue_t *oven_monitor_q;

MAIN()
{
    oven_control_q = q_create(sizeof(baking_program_step_t)*2);
    oven_monitor_q = q_create(sizeof(baking_program_step_t)*2);

    /*Initialize LVGL*/
    lv_init();
    controller_init();

    /*Initialize the HAL (display, input devices, tick) for LVGL*/
    hal_setup();
    ui_init();

    pthread_create(&controller_thread, NULL, controller_thread_function, NULL);

    hal_loop(); // can only be called by main thread

    pthread_join(controller_thread, NULL);
    return 0;
}
