
#ifndef OVEN
#define OVEN

#include <Arduino.h>
#include <lvgl.h>
#include <ui.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

struct {
  bool fanOn;
  bool lightOn;
  bool grillOn;
  bool topHeaterOn;
  bool bottomHeaterOn;
  int temperature;
} ovenState;

struct {
  bool started;
  unsigned long startedAt;
  int setTemperature;
  int setHours;
  int setMinutes;
  int temperature;
  int remainingHours;
  int remainingMinutes;
} program;

void startProgram(lv_event_t * e){
  printf("Starting Program \n");
  program.started = true;
  program.startedAt = millis();
}

void cancelProgram(lv_event_t * e){
  printf("Canceling program \n");
  program.started = false;
}

void toggleFan(lv_event_t * e){
  printf("Toggling fan \n");
}

void toggleGrill(lv_event_t * e){
  printf("Toggling grill \n");
}

void toggleLight(lv_event_t * e){
  printf("Toggling light \n");
}

void stopProgram(lv_event_t * e){
  printf("Stopping program \n");
  program.started = false;
}

void increaseHours(lv_event_t * e){
  printf("Increase hours \n");
  program.setHours += 1;
}

void decreaseHours(lv_event_t * e){
  printf("Decrease hours \n");
  if (program.setHours == 0){
    return;
  }
  program.setHours -= 1;
}

void increaseMinutes(lv_event_t * e){
  printf("Increase minutes \n");
  program.setMinutes += 1;
}

void decreaseMinutes(lv_event_t * e){
  printf("Decrease hours \n");
  if (program.remainingMinutes == 0){
    return;
  }
  program.setMinutes -= 1;
}

void increaseTemperature(lv_event_t * e){
  printf("Increase temperature\n");
  program.setTemperature += 1;
}

void decreaseTemperature(lv_event_t * e){
  printf("Decrease hours \n");
  if (program.setTemperature == 40){
    return;
  }
  program.setTemperature -= 1;
}

void resetOvenState(){
  ovenState.fanOn = false;
  ovenState.lightOn = false;
  ovenState.grillOn = false;
  ovenState.topHeaterOn = false;
  ovenState.bottomHeaterOn = false;
  ovenState.temperature = 0;
}

void oven_task(void *pvParameters){
  resetOvenState();

  unsigned long lastControlAt;
  while(1){
    lastControlAt = millis();
    if (program.started) {
      int totalMinutes = program.setHours * 60 + program.setMinutes;
      int elapsedMinutes = (millis()-program.startedAt)/1000/60;
      program.remainingMinutes = (totalMinutes - elapsedMinutes) % 60;
      program.remainingHours = (totalMinutes - elapsedMinutes) / 60;

      if (program.remainingHours == 0 && program.remainingMinutes == 0){
        printf("\nProgram complete!\n");
        program.started = false;
      }
    }

    // do the oven-control every second
    vTaskDelay(1000/portTICK_RATE_MS);
  }
}


/*--END OF OVEN--*/

#endif /*OVEN*/
