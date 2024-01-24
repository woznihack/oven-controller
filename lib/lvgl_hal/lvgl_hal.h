#pragma once

#if __APPLE__ || defined(WIN32) || defined(__linux__)
#include "sdl2/app_hal.h"
#elif defined(ESP_PLATFORM)
#include "esp32/app_hal.h"
#endif
