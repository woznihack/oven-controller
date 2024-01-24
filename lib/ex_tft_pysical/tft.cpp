/**
 * @file tft.cpp
 *
 */

/** START CODE FOR PHYSICAL DISPLAY */
#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <ui.h>

/* TFT instance (width and height are inverted, as TFT display needs portrait measures) */
TFT_eSPI tft = TFT_eSPI(TFT_HOR_RES, TFT_VER_RES);

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char *buf)
{
  Serial.printf(buf);
  Serial.flush();
}
#endif

/* Display flushing */
void tft_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

/*Read the touchpad*/
void touchpadRead(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  uint16_t touchX = 0, touchY = 0;

  bool touched = tft.getTouch(&touchX, &touchY, 600);

  if (!touched)
  {
    data->state = LV_INDEV_STATE_REL;
  }
  else
  {
    data->state = LV_INDEV_STATE_PR;
    printf("Touch at (%d, %d)\n", touchX, touchY);
    // printf("Inverted touch at (%d, %d)\n", touchY, touchX);
    // printf("Adjusted touch at (%d, %d)\n", TFT_VER_RES - touchX, TFT_VER_RES - touchY);
    data->point.x = touchX;
    data->point.y = touchY;
  }
}

void tft_init(void)
{

  static lv_disp_draw_buf_t draw_buf;
  static lv_color_t buf[TFT_VER_RES * TFT_HOR_RES / 10];

  lv_init();
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, TFT_VER_RES * TFT_HOR_RES / 10);
  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = TFT_VER_RES;
  disp_drv.ver_res = TFT_HOR_RES;
  disp_drv.flush_cb = displayFlush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

}

void touchpad_init(void){
  /*Initialize the input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = touchpadRead;
  lv_indev_drv_register(&indev_drv);
}
