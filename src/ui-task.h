// #include <Arduino.h>
// #include <lvgl.h>
// #include <TFT_eSPI.h>
// #include <ui.h>
// #include "oven-task.h"

// static const uint16_t screenWidth  = 320;
// static const uint16_t screenHeight = 240;

// static lv_disp_draw_buf_t draw_buf;
// static lv_color_t buf[screenWidth * screenHeight / 10];


// /* TFT instance (width and height are inverted, as TFT display needs portrait measures) */
// TFT_eSPI tft = TFT_eSPI(screenHeight, screenWidth);

// #if LV_USE_LOG != 0
// /* Serial debugging */
// void my_print(const char *buf)
// {
//   Serial.printf(buf);
//   Serial.flush();
// }
// #endif

// /* Display flushing */
// void displayFlush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
// {
//   uint32_t w = (area->x2 - area->x1 + 1);
//   uint32_t h = (area->y2 - area->y1 + 1);

//   tft.startWrite();
//   tft.setAddrWindow(area->x1, area->y1, w, h);
//   tft.pushColors((uint16_t *)&color_p->full, w * h, true);
//   tft.endWrite();

//   lv_disp_flush_ready(disp);
// }

// /*Read the touchpad*/
// void touchpadRead(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
// {
//   uint16_t touchX = 0, touchY = 0;

//   bool touched = tft.getTouch(&touchX, &touchY, 600);

//   if (!touched)
//   {
//     data->state = LV_INDEV_STATE_REL;
//   }
//   else
//   {
//     data->state = LV_INDEV_STATE_PR;
//     printf("Touch at (%d, %d)\n", touchX, touchY);
//     // printf("Inverted touch at (%d, %d)\n", touchY, touchX);
//     // printf("Adjusted touch at (%d, %d)\n", screenWidth - touchX, screenWidth - touchY);
//     data->point.x = touchX;
//     data->point.y = touchY;
//   }
// }

// void lv_setup()
// {
//   lv_init();

// #if LV_USE_LOG != 0
//   lv_log_register_print_cb(my_print); /* register print function for debugging */
// #endif

//   lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight / 10);

//   /*Initialize the display*/
//   static lv_disp_drv_t disp_drv;
//   lv_disp_drv_init(&disp_drv);
//   /*Change the following line to your display resolution*/
//   disp_drv.hor_res = screenWidth;
//   disp_drv.ver_res = screenHeight;
//   disp_drv.flush_cb = displayFlush;
//   disp_drv.draw_buf = &draw_buf;
//   lv_disp_drv_register(&disp_drv);

//   /*Initialize the (dummy) input device driver*/
//   static lv_indev_drv_t indev_drv;
//   lv_indev_drv_init(&indev_drv);
//   indev_drv.type = LV_INDEV_TYPE_POINTER;
//   indev_drv.read_cb = touchpadRead;
//   lv_indev_drv_register(&indev_drv);

//   ui_init();
// }


// void ui_task(void *pvParameters)
// {
//   tft.begin();        /* TFT init */
//   tft.setRotation(1); /* Landscape orientation */

//   // uint16_t touchCalibrationData[5] = { 338, 3525, 452, 3233, 6 }; // 240x320 rotation 0
//   uint16_t touchCalibrationData[5] = { 463, 3451, 348, 3511, 3 }; // 240x320 rotation 1
//   // uint16_t touchCalibrationData[5] = { 317, 3540, 441, 3467, 0 }; // 240x320 rotation 2
//   // uint16_t touchCalibrationData[5] = { 440, 3242, 340, 3305, 5 }; // 240x320 rotation 3
//   tft.setTouch(touchCalibrationData);

//   lv_setup();
//   while (1)
//   {
//     if (program.started) {
//       lv_label_set_text_fmt(ui_TemperatureLbl, "%d", program.temperature);
//       lv_label_set_text_fmt(ui_HoursLbl, "%d", program.remainingHours);
//       lv_label_set_text_fmt(ui_MinutesLbl, "%d", program.remainingMinutes);
//     } else {
//       lv_label_set_text_fmt(ui_TemperatureLbl, "%d", program.setTemperature);
//       lv_label_set_text_fmt(ui_HoursLbl, "%d", program.setHours);
//       lv_label_set_text_fmt(ui_MinutesLbl, "%d", program.setMinutes);
//     }

//     lv_task_handler();
//     delay(5);
//   }
// }
