#include "ui.h"

#include <lvgl.h>
#include <stdio.h>

#include "../baking_program.h"
#include "animations.h"
#include "counter.h"
#include "helpers.h"
#include "messages.h"
#include "screens/screens.h"

baking_program_t program;

void ui_init(void) {
  program = baking_program_create("Manual");
  baking_program_add_step(&program, baking_program_step_create());

  splash_scr_init();
  program_setup_scr_init();
  baking_scr_init();

  lv_scr_load(splash_scr);
  lv_scr_load_anim(program_setup_scr, LV_SCR_LOAD_ANIM_FADE_ON, 500, 500, false);
}
