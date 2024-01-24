// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: SquareLine_Project

#include "../ui.h"

void ui_manualMode_screen_init(void)
{
ui_manualMode = lv_obj_create(NULL);
lv_obj_clear_flag( ui_manualMode, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_manualMode, lv_color_hex(0x3E3E3E), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_manualMode, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_StartButton = lv_btn_create(ui_manualMode);
lv_obj_set_width( ui_StartButton, 100);
lv_obj_set_height( ui_StartButton, 38);
lv_obj_set_x( ui_StartButton, -100 );
lv_obj_set_y( ui_StartButton, 91 );
lv_obj_set_align( ui_StartButton, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_StartButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_StartButton, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_StartButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_StartButton, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_StartButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_StartButton, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_width(ui_StartButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_StartButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label2 = lv_label_create(ui_StartButton);
lv_obj_set_width( ui_Label2, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label2, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label2, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label2,"> Start");

ui_BackButton = lv_btn_create(ui_manualMode);
lv_obj_set_width( ui_BackButton, 94);
lv_obj_set_height( ui_BackButton, 38);
lv_obj_set_x( ui_BackButton, 103 );
lv_obj_set_y( ui_BackButton, 91 );
lv_obj_set_align( ui_BackButton, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_BackButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_BackButton, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_BackButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_BackButton, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_BackButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_BackButton, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_width(ui_BackButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_BackButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label6 = lv_label_create(ui_BackButton);
lv_obj_set_width( ui_Label6, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label6, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label6, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label6,"< Back");

ui_TimePanel = lv_obj_create(ui_manualMode);
lv_obj_set_width( ui_TimePanel, 235);
lv_obj_set_height( ui_TimePanel, 83);
lv_obj_set_x( ui_TimePanel, -32 );
lv_obj_set_y( ui_TimePanel, 4 );
lv_obj_set_align( ui_TimePanel, LV_ALIGN_TOP_MID );
lv_obj_clear_flag( ui_TimePanel, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_TimePanel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_TimePanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_TimePanel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_TimePanel, 1, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_HoursPlus = lv_btn_create(ui_TimePanel);
lv_obj_set_width( ui_HoursPlus, 29);
lv_obj_set_height( ui_HoursPlus, 30);
lv_obj_set_x( ui_HoursPlus, -28 );
lv_obj_set_y( ui_HoursPlus, 0 );
lv_obj_set_align( ui_HoursPlus, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_HoursPlus, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_HoursPlus, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_HoursPlus, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_HoursPlus, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_HoursPlus, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_HoursPlus, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_width(ui_HoursPlus, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_HoursPlus, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label10 = lv_label_create(ui_HoursPlus);
lv_obj_set_width( ui_Label10, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label10, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label10, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label10,"+");

ui_HoursMinus = lv_btn_create(ui_TimePanel);
lv_obj_set_width( ui_HoursMinus, 29);
lv_obj_set_height( ui_HoursMinus, 30);
lv_obj_set_x( ui_HoursMinus, -100 );
lv_obj_set_y( ui_HoursMinus, 0 );
lv_obj_set_align( ui_HoursMinus, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_HoursMinus, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_HoursMinus, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_HoursMinus, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_HoursMinus, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_HoursMinus, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_HoursMinus, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_width(ui_HoursMinus, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_HoursMinus, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label11 = lv_label_create(ui_HoursMinus);
lv_obj_set_width( ui_Label11, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label11, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label11, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label11,"-");

ui_MinutesPlus = lv_btn_create(ui_TimePanel);
lv_obj_set_width( ui_MinutesPlus, 29);
lv_obj_set_height( ui_MinutesPlus, 30);
lv_obj_set_x( ui_MinutesPlus, 95 );
lv_obj_set_y( ui_MinutesPlus, 0 );
lv_obj_set_align( ui_MinutesPlus, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_MinutesPlus, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_MinutesPlus, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_MinutesPlus, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_MinutesPlus, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_MinutesPlus, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_MinutesPlus, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_width(ui_MinutesPlus, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_MinutesPlus, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label4 = lv_label_create(ui_MinutesPlus);
lv_obj_set_width( ui_Label4, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label4, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label4, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label4,"+");

ui_MinutesMinus = lv_btn_create(ui_TimePanel);
lv_obj_set_width( ui_MinutesMinus, 29);
lv_obj_set_height( ui_MinutesMinus, 30);
lv_obj_set_x( ui_MinutesMinus, 23 );
lv_obj_set_y( ui_MinutesMinus, 0 );
lv_obj_set_align( ui_MinutesMinus, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_MinutesMinus, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_MinutesMinus, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_MinutesMinus, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_MinutesMinus, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_MinutesMinus, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_MinutesMinus, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_width(ui_MinutesMinus, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_MinutesMinus, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label7 = lv_label_create(ui_MinutesMinus);
lv_obj_set_width( ui_Label7, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label7, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label7, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label7,"-");

ui_HoursLbl = lv_label_create(ui_TimePanel);
lv_obj_set_width( ui_HoursLbl, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_HoursLbl, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_HoursLbl, -63 );
lv_obj_set_y( ui_HoursLbl, 0 );
lv_obj_set_align( ui_HoursLbl, LV_ALIGN_CENTER );
lv_label_set_text(ui_HoursLbl,"00");
lv_obj_set_style_text_color(ui_HoursLbl, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_HoursLbl, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_align(ui_HoursLbl, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_HoursLbl, &lv_font_montserrat_22, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_MinutesLbl = lv_label_create(ui_TimePanel);
lv_obj_set_width( ui_MinutesLbl, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_MinutesLbl, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_MinutesLbl, 59 );
lv_obj_set_y( ui_MinutesLbl, 0 );
lv_obj_set_align( ui_MinutesLbl, LV_ALIGN_CENTER );
lv_label_set_text(ui_MinutesLbl,"60");
lv_obj_set_style_text_color(ui_MinutesLbl, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_MinutesLbl, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_align(ui_MinutesLbl, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_MinutesLbl, &lv_font_montserrat_22, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Separator = lv_label_create(ui_TimePanel);
lv_obj_set_width( ui_Separator, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Separator, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Separator, -2 );
lv_obj_set_y( ui_Separator, 0 );
lv_obj_set_align( ui_Separator, LV_ALIGN_CENTER );
lv_label_set_text(ui_Separator,":");
lv_obj_set_style_text_color(ui_Separator, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_Separator, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_align(ui_Separator, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_Separator, &lv_font_montserrat_22, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_TemperaturePanel = lv_obj_create(ui_manualMode);
lv_obj_set_width( ui_TemperaturePanel, 236);
lv_obj_set_height( ui_TemperaturePanel, 92);
lv_obj_set_x( ui_TemperaturePanel, -31 );
lv_obj_set_y( ui_TemperaturePanel, 93 );
lv_obj_set_align( ui_TemperaturePanel, LV_ALIGN_TOP_MID );
lv_obj_clear_flag( ui_TemperaturePanel, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_TemperaturePanel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_TemperaturePanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_TemperaturePanel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_TemperaturePanel, 1, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_TemperatureLbl = lv_label_create(ui_TemperaturePanel);
lv_obj_set_width( ui_TemperatureLbl, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_TemperatureLbl, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_TemperatureLbl, 4 );
lv_obj_set_y( ui_TemperatureLbl, -1 );
lv_obj_set_align( ui_TemperatureLbl, LV_ALIGN_CENTER );
lv_label_set_text(ui_TemperatureLbl,"170");
lv_obj_set_style_text_color(ui_TemperatureLbl, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_TemperatureLbl, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_align(ui_TemperatureLbl, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_TemperatureLbl, &lv_font_montserrat_48, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_PlusTemperature = lv_btn_create(ui_TemperaturePanel);
lv_obj_set_width( ui_PlusTemperature, 29);
lv_obj_set_height( ui_PlusTemperature, 30);
lv_obj_set_x( ui_PlusTemperature, 81 );
lv_obj_set_y( ui_PlusTemperature, 0 );
lv_obj_set_align( ui_PlusTemperature, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_PlusTemperature, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_PlusTemperature, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_PlusTemperature, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_PlusTemperature, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_PlusTemperature, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_PlusTemperature, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_width(ui_PlusTemperature, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_PlusTemperature, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label5 = lv_label_create(ui_PlusTemperature);
lv_obj_set_width( ui_Label5, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label5, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label5, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label5,"+");

ui_MinusTemperature = lv_btn_create(ui_TemperaturePanel);
lv_obj_set_width( ui_MinusTemperature, 29);
lv_obj_set_height( ui_MinusTemperature, 30);
lv_obj_set_x( ui_MinusTemperature, -79 );
lv_obj_set_y( ui_MinusTemperature, 0 );
lv_obj_set_align( ui_MinusTemperature, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_MinusTemperature, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_MinusTemperature, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_MinusTemperature, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_MinusTemperature, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_MinusTemperature, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_MinusTemperature, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_width(ui_MinusTemperature, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_MinusTemperature, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label8 = lv_label_create(ui_MinusTemperature);
lv_obj_set_width( ui_Label8, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label8, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label8, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label8,"-");

ui_Fan = lv_btn_create(ui_manualMode);
lv_obj_set_width( ui_Fan, 61);
lv_obj_set_height( ui_Fan, 38);
lv_obj_set_x( ui_Fan, 123 );
lv_obj_set_y( ui_Fan, -97 );
lv_obj_set_align( ui_Fan, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_Fan, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_Fan, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_Fan, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_Fan, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_Fan, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_Fan, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_width(ui_Fan, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_Fan, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label9 = lv_label_create(ui_Fan);
lv_obj_set_width( ui_Label9, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label9, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label9, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label9,"Fan");

ui_Grill = lv_btn_create(ui_manualMode);
lv_obj_set_width( ui_Grill, 61);
lv_obj_set_height( ui_Grill, 38);
lv_obj_set_x( ui_Grill, 123 );
lv_obj_set_y( ui_Grill, -54 );
lv_obj_set_align( ui_Grill, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_Grill, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_Grill, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_Grill, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_Grill, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_Grill, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_Grill, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_width(ui_Grill, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_Grill, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label12 = lv_label_create(ui_Grill);
lv_obj_set_width( ui_Label12, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label12, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label12, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label12,"Grill");

ui_Light = lv_btn_create(ui_manualMode);
lv_obj_set_width( ui_Light, 61);
lv_obj_set_height( ui_Light, 38);
lv_obj_set_x( ui_Light, 123 );
lv_obj_set_y( ui_Light, -10 );
lv_obj_set_align( ui_Light, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_Light, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_Light, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_Light, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_Light, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_Light, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_Light, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_width(ui_Light, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_Light, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label13 = lv_label_create(ui_Light);
lv_obj_set_width( ui_Label13, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label13, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label13, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label13,"Light");

ui_StopButton = lv_btn_create(ui_manualMode);
lv_obj_set_width( ui_StopButton, 100);
lv_obj_set_height( ui_StopButton, 38);
lv_obj_set_x( ui_StopButton, 3 );
lv_obj_set_y( ui_StopButton, 91 );
lv_obj_set_align( ui_StopButton, LV_ALIGN_CENTER );
lv_obj_add_state( ui_StopButton, LV_STATE_DISABLED );     /// States
lv_obj_add_flag( ui_StopButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_StopButton, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_StopButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_bg_color(ui_StopButton, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_StopButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_StopButton, 1, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_width(ui_StopButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_StopButton, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label3 = lv_label_create(ui_StopButton);
lv_obj_set_width( ui_Label3, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label3, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label3, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label3,"x Stop");

lv_obj_add_event_cb(ui_StartButton, ui_event_StartButton, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_BackButton, ui_event_BackButton, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_HoursPlus, ui_event_HoursPlus, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_HoursMinus, ui_event_HoursMinus, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_MinutesPlus, ui_event_MinutesPlus, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_MinutesMinus, ui_event_MinutesMinus, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_PlusTemperature, ui_event_PlusTemperature, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_MinusTemperature, ui_event_MinusTemperature, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_Fan, ui_event_Fan, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_Grill, ui_event_Grill, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_Light, ui_event_Light, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_StopButton, ui_event_StopButton, LV_EVENT_ALL, NULL);

}
