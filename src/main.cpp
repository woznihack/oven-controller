#include <runner.h>

#include <lv_conf.h>
#include <lvgl_hal.h>
#include <stdio.h>
#include <lvgl.h>
#include <unistd.h>
MAIN()
{
    /*Initialize LVGL*/
    lv_init();

    /*Initialize the HAL (display, input devices, tick) for LVGL*/
    hal_setup();

    /* use a pretty small demo for monochrome displays */
    /* Get the current screen  */
    lv_obj_t *scr = lv_disp_get_scr_act(NULL);

    lv_obj_t *ui_onButton = lv_btn_create(scr);
    lv_obj_set_width( ui_onButton, 228);
    lv_obj_set_height( ui_onButton, 30);
    lv_obj_set_x( ui_onButton, -4 );
    lv_obj_set_y( ui_onButton, -8 );
    lv_obj_set_align( ui_onButton, LV_ALIGN_CENTER );
    lv_obj_add_flag( ui_onButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
    lv_obj_clear_flag( ui_onButton, LV_OBJ_FLAG_SCROLLABLE );    /// Flags


    /*Create a Label on the currently active screen*/
    lv_obj_t *label1 = lv_label_create(scr);
    lv_label_set_text(label1, "Hello\nworld");

    /* Align the Label to the center
     * NULL means align on parent (which is the screen now)
     * 0, 0 at the end means an x, y offset after alignment*/
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, 0);

    hal_loop();

    return 0;
}
