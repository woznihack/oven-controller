#if defined(ESP_PLATFORM) || defined(ARDUINO)

#include <lvgl.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_task_wdt.h>
#include <TFT_eSPI.h>

#include "app_hal.h"

#define LV_TICK_PERIOD_MS 1

/* Prototypes */
void tft_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
static void lv_tick_task(void *arg);
/* ---------- */

/* Globals    */
TFT_eSPI tft = TFT_eSPI(TFT_WIDTH, TFT_HEIGHT);
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[TFT_HEIGHT * TFT_WIDTH / 10];
/* ---------- */

void hal_setup(void)
{
  printf("starting HAL setup TFT(%d, %d)\r\n", TFT_WIDTH, TFT_HEIGHT);

  Serial.begin(115200);
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, TFT_HEIGHT * TFT_WIDTH / 10); /*Initialize the display buffer*/

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);   /*Basic initialization*/
  disp_drv.flush_cb = tft_flush; /*Used when `LV_VDB_SIZE != 0` in lv_conf.h (buffered drawing)*/
  disp_drv.draw_buf = &draw_buf;
  disp_drv.hor_res = TFT_HEIGHT;
  disp_drv.ver_res = TFT_WIDTH;
  lv_disp_drv_register(&disp_drv);

  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.read_cb = touchpad_read;
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  lv_indev_drv_register(&indev_drv);

  const esp_timer_create_args_t periodic_timer_args = {
      .callback = &lv_tick_task,
      .name = "periodic_gui"};

  static esp_timer_handle_t periodic_timer;
  ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
  ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));
  tft.begin();
  tft.setRotation(0);

  uint16_t touchCalibrationData[5] = { 338, 3525, 452, 3233, 6 }; // 240x320 rotation 0
  // uint16_t touchCalibrationData[5] = {463, 3451, 348, 3511, 3}; // 240x320 rotation 1
  // uint16_t touchCalibrationData[5] = { 317, 3540, 441, 3467, 0 }; // 240x320 rotation 2
  // uint16_t touchCalibrationData[5] = { 440, 3242, 340, 3305, 5 }; // 240x320 rotation 3
  tft.setTouch(touchCalibrationData);
}

void hal_loop(void)
{
  printf("starting HAL loop\r\n");
  while (1)
  {
    uint32_t next = lv_task_handler();
    vTaskDelay(pdMS_TO_TICKS(next));
    esp_task_wdt_reset();
  }
}


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
void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
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
    Serial.printf("Touch at (%d, %d)\n", touchX, touchY);
    // printf("Inverted touch at (%d, %d)\n", touchY, touchX);
    // printf("Adjusted touch at (%d, %d)\n", TFT_HEIGHT - touchX, TFT_HEIGHT - touchY);
    data->point.x = touchX;
    data->point.y = touchY;
  }
}

static void lv_tick_task(void *arg)
{
  lv_tick_inc(LV_TICK_PERIOD_MS);
}


#endif
