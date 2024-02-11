#include <stdio.h>
#include <lvgl.h>

#include "ui.h"
#include "counter.h"
#include "animations.h"
#include "helpers.h"
#include "messages.h"
#include "program.h"
#include "screens/screens.h"

// prototypes
// static void handle_oven_monitor_q(lv_timer_t *);
static void handle_ui_msg(void *s, lv_msg_t *msg);

program_t program;

void ui_init(void)
{
  program = program_create("Manual");

  splash_scr_init();
  program_setup_scr_init();
  baking_scr_init();

  lv_disp_load_scr(splash_scr);
  lv_scr_load_anim(program_setup_scr, LV_SCR_LOAD_ANIM_FADE_ON, 200, 200, true);
  // lv_timer_t *main_screen_update_timer = lv_timer_create(handle_oven_monitor_q, 100, NULL);
  lv_msg_subscribe(MSG_SET_STATUS_STARTED, handle_ui_msg, NULL);
  lv_msg_subscribe(MSG_SET_STATUS_STOPPED, handle_ui_msg, NULL);
}

void handle_ui_msg(void *s, lv_msg_t *msg)
{
  uint32_t id = lv_msg_get_id(msg);
  switch (id)
  {
  case MSG_SET_STATUS_STARTED:
  {
    if (baking_scr == NULL)
    {
      baking_scr_init();
    }
    lv_scr_load_anim(baking_scr, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 300, false);

    uint16_t baking_steps_count = program.last_step_idx + 1;
    q_enqueue(oven_control_q, CONTROL_OVEN_SET_BAKING_STEPS_COUNT, &baking_steps_count);
    q_enqueue(oven_control_q, CONTROL_OVEN_SET_BAKING_STEPS, &program.steps);
    q_enqueue(oven_control_q, CONTROL_OVEN_START, NULL);
    break;
  }
  case MSG_SET_STATUS_STOPPED:
    // lv_scr_load_anim(program_setup_scr, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 300, false);
    q_enqueue(oven_control_q, CONTROL_OVEN_STOP, NULL);
    break;
  default:
    break;
  }
}
