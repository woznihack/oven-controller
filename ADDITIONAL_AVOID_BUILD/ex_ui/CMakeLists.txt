SET(SOURCES screens/ui_mainScreen.c
    screens/ui_manualMode.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    ui_events.c)

add_library(ui ${SOURCES})
