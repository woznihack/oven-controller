#pragma once



#if defined(ARDUINO) || defined(ESP_PLATFORM)

#define MAIN() \
    int call_app_main();\
    extern "C" void app_main(){\
        call_app_main();\
    }\
    int call_app_main()
#else
    #define MAIN() \
    int main(int argc, char **argv)
#endif
