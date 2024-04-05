// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: SquareLine_Project

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////


// SCREEN: ui_mainScreen
void ui_mainScreen_screen_init(void);
lv_obj_t *ui_mainScreen;
void ui_event_onButton( lv_event_t * e);
lv_obj_t *ui_onButton;
lv_obj_t *ui_onButtonLabel;
lv_obj_t *ui_Label1;


// SCREEN: ui_manualMode
void ui_manualMode_screen_init(void);
lv_obj_t *ui_manualMode;
void ui_event_StartButton( lv_event_t * e);
lv_obj_t *ui_StartButton;
lv_obj_t *ui_Label2;
void ui_event_BackButton( lv_event_t * e);
lv_obj_t *ui_BackButton;
lv_obj_t *ui_Label6;
lv_obj_t *ui_TimePanel;
void ui_event_HoursPlus( lv_event_t * e);
lv_obj_t *ui_HoursPlus;
lv_obj_t *ui_Label10;
void ui_event_HoursMinus( lv_event_t * e);
lv_obj_t *ui_HoursMinus;
lv_obj_t *ui_Label11;
void ui_event_MinutesPlus( lv_event_t * e);
lv_obj_t *ui_MinutesPlus;
lv_obj_t *ui_Label4;
void ui_event_MinutesMinus( lv_event_t * e);
lv_obj_t *ui_MinutesMinus;
lv_obj_t *ui_Label7;
lv_obj_t *ui_HoursLbl;
lv_obj_t *ui_MinutesLbl;
lv_obj_t *ui_Separator;
lv_obj_t *ui_TemperaturePanel;
lv_obj_t *ui_TemperatureLbl;
void ui_event_PlusTemperature( lv_event_t * e);
lv_obj_t *ui_PlusTemperature;
lv_obj_t *ui_Label5;
void ui_event_MinusTemperature( lv_event_t * e);
lv_obj_t *ui_MinusTemperature;
lv_obj_t *ui_Label8;
void ui_event_Fan( lv_event_t * e);
lv_obj_t *ui_Fan;
lv_obj_t *ui_Label9;
void ui_event_Grill( lv_event_t * e);
lv_obj_t *ui_Grill;
lv_obj_t *ui_Label12;
void ui_event_Light( lv_event_t * e);
lv_obj_t *ui_Light;
lv_obj_t *ui_Label13;
void ui_event_StopButton( lv_event_t * e);
lv_obj_t *ui_StopButton;
lv_obj_t *ui_Label3;
lv_obj_t *ui____initial_actions0;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
void ui_event_onButton( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      _ui_screen_change( &ui_manualMode, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_manualMode_screen_init);
}
}
void ui_event_StartButton( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      startProgram( e );
      _ui_state_modify( ui_MinutesPlus, LV_STATE_DISABLED, _UI_MODIFY_STATE_ADD);
      _ui_state_modify( ui_MinutesMinus, LV_STATE_DISABLED, _UI_MODIFY_STATE_ADD);
      _ui_state_modify( ui_HoursPlus, LV_STATE_DISABLED, _UI_MODIFY_STATE_ADD);
      _ui_state_modify( ui_HoursMinus, LV_STATE_DISABLED, _UI_MODIFY_STATE_ADD);
      _ui_state_modify( ui_StopButton, LV_STATE_DISABLED, _UI_MODIFY_STATE_REMOVE);
      _ui_state_modify( ui_MinusTemperature, LV_STATE_DISABLED, _UI_MODIFY_STATE_ADD);
      _ui_state_modify( ui_PlusTemperature, LV_STATE_DISABLED, _UI_MODIFY_STATE_ADD);
      _ui_state_modify( ui_StartButton, LV_STATE_DISABLED, _UI_MODIFY_STATE_ADD);
}
}
void ui_event_BackButton( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      cancelProgram( e );
      _ui_screen_change( &ui_mainScreen, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_mainScreen_screen_init);
}
}
void ui_event_HoursPlus( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      increaseHours( e );
}
}
void ui_event_HoursMinus( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      decreaseHours( e );
}
}
void ui_event_MinutesPlus( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      increaseMinutes( e );
}
}
void ui_event_MinutesMinus( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      decreaseMinutes( e );
}
}
void ui_event_PlusTemperature( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      increaseTemperature( e );
}
}
void ui_event_MinusTemperature( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      decreaseTemperature( e );
}
}
void ui_event_Fan( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      toggleFan( e );
}
}
void ui_event_Grill( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      toggleGrill( e );
}
}
void ui_event_Light( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      toggleLight( e );
}
}
void ui_event_StopButton( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      stopProgram( e );
      _ui_state_modify( ui_MinutesPlus, LV_STATE_DISABLED, _UI_MODIFY_STATE_REMOVE);
      _ui_state_modify( ui_MinutesMinus, LV_STATE_DISABLED, _UI_MODIFY_STATE_REMOVE);
      _ui_state_modify( ui_HoursPlus, LV_STATE_DISABLED, _UI_MODIFY_STATE_TOGGLE);
      _ui_state_modify( ui_HoursMinus, LV_STATE_DISABLED, _UI_MODIFY_STATE_TOGGLE);
      _ui_state_modify( ui_StartButton, LV_STATE_DISABLED, _UI_MODIFY_STATE_REMOVE);
      _ui_state_modify( ui_MinusTemperature, LV_STATE_DISABLED, _UI_MODIFY_STATE_REMOVE);
      _ui_state_modify( ui_PlusTemperature, LV_STATE_DISABLED, _UI_MODIFY_STATE_REMOVE);
      _ui_state_modify( ui_StopButton, LV_STATE_DISABLED, _UI_MODIFY_STATE_ADD);
}
}

///////////////////// SCREENS ////////////////////

void ui_init( void )
{
lv_disp_t *dispp = lv_disp_get_default();
lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
lv_disp_set_theme(dispp, theme);
ui_mainScreen_screen_init();
ui_manualMode_screen_init();
ui____initial_actions0 = lv_obj_create(NULL);
lv_disp_load_scr( ui_mainScreen);
}