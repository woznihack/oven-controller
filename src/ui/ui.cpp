#include "ui.h"

#include <lvgl.h>
#include <stdio.h>

#include "../baking_program.h"
#include "animations.h"
#include "counter.h"
#include "messages.h"
#include "screens/screens.h"
#include "screens/styles.h"

baking_program_t program;

void ui_init(void) {
  char program_name[] = "Manual";
  program = baking_program_create(program_name);

  init_styles();
  splash_scr_init();
  program_setup_scr_init();
  baking_scr_init();

  lv_scr_load(splash_scr);
  lv_scr_load_anim(program_setup_scr, LV_SCR_LOAD_ANIM_FADE_ON, 500, 500, false);
}
