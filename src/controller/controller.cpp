#include "controller.h"

// HELPERS
void sleep_ms(int ms) { usleep(1000 * ms); }
void print_info();
void send_monitor_data();

// PROTOTYPES
void controller_init();
void controller_loop();
void handle_queue_messages();
void read_probes();
void oven_idle();
void oven_preheat();
void oven_mantain();
void oven_cooldown();
void light_on();
void light_off();
void fan_on();
void fan_off();
void top_heater_on();
void top_heater_off();
void bottom_heater_on();
void bottom_heater_off();

// GLOBALS
oven_state_node_t oven_sm[] = { // oven state machine
    {OVEN_S_IDLE, oven_idle},
    {OVEN_S_PREHEAT, oven_preheat},
    {OVEN_S_MANTAIN, oven_mantain},
    {OVEN_S_COOLDOWN, oven_cooldown}};

oven_light_state_node_t light_sm[] = {
    // light state machine
    {OVEN_S_LIGHT_ON, light_on},
    {OVEN_S_LIGHT_OFF, light_off},
};

oven_fan_state_node_t fan_sm[] = {
    // fan state machine
    {OVEN_S_FAN_ON, fan_on},
    {OVEN_S_FAN_OFF, fan_off},
};

oven_top_heater_state_node_t top_heater_sm[] = {
    // light state machine
    {OVEN_S_TOP_HEATER_ON, top_heater_on},
    {OVEN_S_TOP_HEATER_OFF, top_heater_off},
};

oven_bottom_heater_state_node_t bottom_heater_sm[] = {
    // light state machine
    {OVEN_S_BOTTOM_HEATER_ON, bottom_heater_on},
    {OVEN_S_BOTTOM_HEATER_OFF, bottom_heater_off},
};

oven_state_t current_oven_state = OVEN_S_IDLE;
oven_light_state_t current_light_state = OVEN_S_LIGHT_OFF;
oven_fan_state_t current_fan_state = OVEN_S_FAN_OFF;
oven_top_heater_state_t current_top_heater_state = OVEN_S_TOP_HEATER_OFF;
oven_bottom_heater_state_t current_bottom_heater_state = OVEN_S_BOTTOM_HEATER_OFF;

uint16_t top_heater_temperature;
uint16_t bottom_heater_temperature;
uint16_t cooking_duration_m = 0;
uint16_t cooking_temperature = 0;

time_t loop_started_at;
time_t preheat_started_at;
time_t cooking_started_at;
time_t cooldown_started_at;

// for debugging
const char *oven_state_strings[] = {"IDLE", "PREHEAT", "MANTAIN", "COOLDOWN"};
const char *light_state_strings[] = {"ON", "OFF"};
const char *fan_state_strings[] = {"ON", "OFF"};
const char *top_heater_state_strings[] = {"ON", "OFF"};
const char *bottom_heater_state_string[] = {"ON", "OFF"};

void controller_init() {}

void controller_loop()
{
  loop_started_at = time(NULL);

  while (1)
  {
    handle_queue_messages();
    read_probes();

    (oven_sm[current_oven_state].stateFunc)();
    (fan_sm[current_fan_state].stateFunc)();
    (light_sm[current_light_state].stateFunc)();
    (top_heater_sm[current_top_heater_state].stateFunc)();
    (bottom_heater_sm[current_bottom_heater_state].stateFunc)();

    print_info();
    send_monitor_data();
    sleep_ms(1000);
  }
}

void oven_idle()
{
  current_top_heater_state = OVEN_S_TOP_HEATER_OFF;
  current_bottom_heater_state = OVEN_S_BOTTOM_HEATER_OFF;

  // todo: find a way to not do this every iteration
}

void oven_preheat()
{
  if (preheat_started_at == 0)
  {
    preheat_started_at = time(NULL);
    current_top_heater_state = OVEN_S_TOP_HEATER_ON;
    current_bottom_heater_state = OVEN_S_BOTTOM_HEATER_ON;

    // tell UI preheat started
    q_enqueue(ui_queue, UI_UPDATE_BAKING_PHASE, "preheating");
    printf("[OVEN] PREHEAT started\n");
  }

  // EMULATION: increase temperature of 1 tenth every second
  double elapsed_seconds = difftime(time(NULL), preheat_started_at);
  top_heater_temperature = floor((double)cooking_temperature/10 * elapsed_seconds);
  bottom_heater_temperature = floor((double)cooking_temperature/10 * elapsed_seconds);



  bool preheat_complete = (top_heater_temperature >= cooking_temperature &&
                           bottom_heater_temperature >= cooking_temperature);
  if (preheat_complete)
  {
    preheat_started_at = 0;
    current_oven_state = OVEN_S_MANTAIN;

    // tell UI preheat complete

    printf("[OVEN] PREHEAT complete\n");
  }
  else
  {
    // do nothing
  }
}

void oven_mantain()
{
  if (cooking_started_at == 0)
  {
    cooking_started_at = time(NULL);
    current_top_heater_state = OVEN_S_TOP_HEATER_ON;
    current_bottom_heater_state = OVEN_S_BOTTOM_HEATER_ON;

    // tell UI mantain started
    q_enqueue(ui_queue, UI_UPDATE_BAKING_PHASE, "baking");
    printf("[OVEN] MANTAIN started\n");
  }

  int elapsed_m = (int)floor(difftime(time(NULL), cooking_started_at) / 60);
  int remaining_m = cooking_duration_m - elapsed_m;
  if (remaining_m == 0)
  {
    cooking_started_at = 0;
    current_oven_state = OVEN_S_COOLDOWN;
    current_top_heater_state = OVEN_S_TOP_HEATER_OFF;
    current_bottom_heater_state = OVEN_S_BOTTOM_HEATER_OFF;

    // tell UI mantain complete
    printf("[OVEN] MANTAIN complete\n");
  }

  q_enqueue(ui_queue, UI_UPDATE_REMAINING_M, &remaining_m);
}

void oven_cooldown()
{
  if (cooldown_started_at == 0)
  {
    cooldown_started_at = time(NULL);
    current_top_heater_state = OVEN_S_TOP_HEATER_OFF;
    current_bottom_heater_state = OVEN_S_BOTTOM_HEATER_OFF;

    // tell UI cooldown started
    q_enqueue(ui_queue, UI_UPDATE_BAKING_PHASE, "cooldown");
    printf("[OVEN] COOLDOWN started\n");
  }

  // EMULATE COOLDOWN COMPLETE JUST IF 30 seconds passed from beginning
  bool cooldown_complete = (difftime(time(NULL), cooldown_started_at) >= 30);
  if (cooldown_complete)
  {
    current_oven_state = OVEN_S_IDLE;

    // tell UI cooldown ended
    printf("[OVEN] COOLDOWN complete\n");
  }
}

void light_on() {}
void light_off() {}
void fan_on() {}
void fan_off() {}
void top_heater_on() {}
void top_heater_off() {}
void bottom_heater_on() {}
void bottom_heater_off() {}

void read_probes() {}
void print_info()
{
  int diff_seconds = (int)difftime(time(NULL), loop_started_at);
  if (diff_seconds % 10 == 0)
  {
    printf("[OVEN] State dump\n");
    printf("o - oven_state    = %s\n", oven_state_strings[current_oven_state]);
    printf("o - fan           = %s\n", fan_state_strings[current_fan_state]);
    printf("o - light         = %s\n", light_state_strings[current_light_state]);
    printf("o - top_heater    = %s\n", top_heater_state_strings[current_top_heater_state]);
    printf("o - bottom_heater = %s\n", bottom_heater_state_string[current_bottom_heater_state]);
    printf("o --------------------\n");
    printf("o - top_heater_t  = %d C\n", top_heater_temperature);
    printf("o - bot_heater_t  = %d C\n", bottom_heater_temperature);
  }
}

void send_monitor_data(){
  if (current_oven_state == OVEN_S_MANTAIN){
    printf("Sending monitor data \n");
    int remaining_m = cooking_duration_m - (int)floor(difftime(time(NULL), cooking_started_at) / 60);
    q_enqueue(ui_queue, UI_UPDATE_REMAINING_M, &remaining_m);
    q_enqueue(ui_queue, UI_UPDATE_TOP_HEATER_TEMP, &top_heater_temperature);
    q_enqueue(ui_queue, UI_UPDATE_DECK_HEATER_TEMP, &bottom_heater_temperature);
  }
}
void handle_queue_messages()
{
  q_node_t data;
  while (q_dequeue(oven_queue, &data))
  {
    printf("[OVEN] Handling event code = %d\n", data.event);
    if (data.event == OVEN_START)
    {
      current_oven_state = OVEN_S_PREHEAT;
    }
    if (data.event == OVEN_STOP)
    {
      current_oven_state = OVEN_S_IDLE;
    }
    if (data.event == OVEN_SET_TEMPERATURE)
    {
      cooking_temperature = *(int *)data.payload;
    }
    if (data.event == OVEN_SET_DURATION_M)
    {
      cooking_duration_m = *(int *)data.payload;
    }
  }
}
