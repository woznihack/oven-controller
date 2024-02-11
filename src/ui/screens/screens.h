#ifndef SCREENS_H
#define SCREENS_H

#include "../ui.h"
#include "../program.h"

extern lv_obj_t *splash_scr;
extern lv_obj_t *program_setup_scr;
extern lv_obj_t *baking_scr;

void splash_scr_init();
void program_setup_scr_init();
void baking_scr_init();

extern program_t program;
#endif
