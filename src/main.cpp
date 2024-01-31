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

program_t * oven_program;
pthread_t controller_thread;
void * controller_thread_function(void *ptr){
    long lastControlAt;
    while(1){
        lastControlAt = time(NULL);
        if (oven_program->started) {
            int elapsedMinutes = (time(NULL)-oven_program->startedAt)/60;
            oven_program->remainingMinutes = oven_program->setDurationMinutes - elapsedMinutes;

            if (oven_program->remainingMinutes <= 0){
                printf("\nProgram complete!\n");
                oven_program->started = false;
            }
        }
        sleep(1);
    }
}

void start_program(lv_event_t * t){
    printf("Starting program\n");
    oven_program->remainingMinutes = oven_program->setDurationMinutes;
    oven_program->started = true;
    oven_program->startedAt = time(NULL);
}

void stop_program(lv_event_t * t){
    printf("Stoping program\n");
    oven_program->started = false;
}

MAIN()
{
    oven_program = create_oven_program(strdup("Manual"), 180, 40);


    /*Initialize LVGL*/
    lv_init();

    /*Initialize the HAL (display, input devices, tick) for LVGL*/
    hal_setup();


    ui_init();

    set_grid_btn_cb(0, start_program);
    set_grid_btn_cb(1, stop_program);


    pthread_create(&controller_thread, NULL, controller_thread_function, NULL);

    hal_loop(); // can only be called by main thread

    pthread_join( controller_thread, NULL);

    return 0;
}
