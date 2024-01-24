// Configured by Dror Gluska

#ifndef SPI_HOST_MAX
#define SPI_HOST_MAX 4
#endif

#define CONFIG_LV_PREDEFINED_DISPLAY_NONE 1
#define CONFIG_LV_TFT_DISPLAY_CONTROLLER_ILI9488 1
#define CONFIG_LV_TFT_DISPLAY_PROTOCOL_SPI 1
#define CONFIG_LV_DISPLAY_ORIENTATION_LANDSCAPE_INVERTED 1
#define CONFIG_LV_DISPLAY_ORIENTATION 3
#define CONFIG_LV_PREDEFINED_PINS_38V4 1
#define CONFIG_LV_TFT_DISPLAY_USER_CONTROLLER_ILI9488 1
#define CONFIG_CUSTOM_DISPLAY_BUFFER_SIZE 1
#define CONFIG_CUSTOM_DISPLAY_BUFFER_BYTES 1024
#define CONFIG_LV_TFT_DISPLAY_SPI2_HOST 1
#define CONFIG_LV_TFT_DISPLAY_SPI_TRANS_MODE_SIO 1
#define CONFIG_LV_TFT_DISPLAY_SPI_FULL_DUPLEX 1
#define CONFIG_LV_TFT_CUSTOM_SPI_CLK_DIVIDER 2
#define CONFIG_LV_DISP_SPI_MOSI 13
#define CONFIG_LV_DISPLAY_USE_SPI_MISO 1
#define CONFIG_LV_DISP_SPI_MISO 12
#define CONFIG_LV_DISP_SPI_INPUT_DELAY_NS 0
#define CONFIG_LV_DISP_SPI_CLK 14
#define CONFIG_LV_DISPLAY_USE_SPI_CS 1
#define CONFIG_LV_DISP_SPI_CS 5
#define CONFIG_LV_DISPLAY_USE_DC 1
#define CONFIG_LV_DISP_PIN_DC 19
#define CONFIG_LV_DISP_USE_RST 1
#define CONFIG_LV_DISP_PIN_RST 18
#define CONFIG_LV_DISP_BACKLIGHT_PWM 1
#define CONFIG_LV_BACKLIGHT_ACTIVE_LVL 1
#define CONFIG_LV_DISP_PIN_BCKL 23
#define CONFIG_LV_I2C_DISPLAY_PORT 0
#define CONFIG_LV_TOUCH_CONTROLLER 1
#define CONFIG_LV_TOUCH_CONTROLLER_XPT2046 1
#define CONFIG_LV_TOUCH_DRIVER_PROTOCOL_SPI 1
#define CONFIG_LV_TOUCH_CONTROLLER_SPI2_HOST 1
#define CONFIG_LV_TOUCH_SPI_MISO 12
#define CONFIG_LV_TOUCH_SPI_MOSI 13
#define CONFIG_LV_TOUCH_SPI_CLK 14
#define CONFIG_LV_TOUCH_SPI_CS 25
#define CONFIG_LV_TOUCH_PIN_IRQ 34
#define CONFIG_LV_TOUCH_X_MIN 200
#define CONFIG_LV_TOUCH_Y_MIN 120
#define CONFIG_LV_TOUCH_X_MAX 1900
#define CONFIG_LV_TOUCH_Y_MAX 1900
#define CONFIG_LV_TOUCH_DETECT_IRQ 1
#define CONFIG_LV_I2C_TOUCH_PORT 0
