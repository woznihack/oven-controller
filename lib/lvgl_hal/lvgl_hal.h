#pragma once

#if defined(ARDUINO) || defined(ESP_PLATFORM)
#include "esp32/app_hal.h"
#endif

#if defined(_WIN64) || defined(_WIN32) || defined(__APPLE__) || defined(__linux__)
#include "sdl2/app_hal.h"
#endif
