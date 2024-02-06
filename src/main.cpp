#include <runner.h>
#include <pthread.h>
#include <lv_conf.h>
#include <lvgl_hal.h>
#include <stdio.h>
#include <lvgl.h>
#include <unistd.h>
#include <time.h>

#include "ui/ui.h"
#include "controller/controller.h"

pthread_t controller_thread;
void * controller_thread_function(void *ptr){
    controller_loop();
}

q_queue_t * oven_queue;
q_queue_t * ui_queue;

MAIN()
{
    oven_queue = q_create(sizeof(char)*10);
    ui_queue   = q_create(sizeof(char)*10);

    /*Initialize LVGL*/
    lv_init();
    controller_init();

    /*Initialize the HAL (display, input devices, tick) for LVGL*/
    hal_setup();
    ui_init();

    pthread_create(&controller_thread, NULL, controller_thread_function, NULL);

    hal_loop(); // can only be called by main thread

    pthread_join( controller_thread, NULL);
    return 0;
}
