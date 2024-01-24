// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: SquareLine_Project

#include "../ui.h"

void ui_mainScreen_screen_init(void)
{
ui_mainScreen = lv_obj_create(NULL);
lv_obj_clear_flag( ui_mainScreen, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_mainScreen, lv_color_hex(0x3E3E3E), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_mainScreen, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_onButton = lv_btn_create(ui_mainScreen);
lv_obj_set_width( ui_onButton, 228);
lv_obj_set_height( ui_onButton, 30);
lv_obj_set_x( ui_onButton, -4 );
lv_obj_set_y( ui_onButton, -8 );
lv_obj_set_align( ui_onButton, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_onButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_onButton, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_onButtonLabel = lv_label_create(ui_onButton);
lv_obj_set_width( ui_onButtonLabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_onButtonLabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_onButtonLabel, LV_ALIGN_CENTER );
lv_label_set_text(ui_onButtonLabel,"Manual");

ui_Label1 = lv_label_create(ui_mainScreen);
lv_obj_set_width( ui_Label1, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label1, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Label1, 0 );
lv_obj_set_y( ui_Label1, 10 );
lv_obj_set_align( ui_Label1, LV_ALIGN_TOP_MID );
lv_label_set_text(ui_Label1,"Oven Controller");
lv_obj_set_style_text_color(ui_Label1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_Label1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_Label1, &lv_font_montserrat_18, LV_PART_MAIN| LV_STATE_DEFAULT);

lv_obj_add_event_cb(ui_onButton, ui_event_onButton, LV_EVENT_ALL, NULL);

}
