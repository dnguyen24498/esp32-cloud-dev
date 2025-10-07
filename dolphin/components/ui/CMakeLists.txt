idf_component_register(SRCS
    "drivers/lcd_st7567.c"
    "gfx/lcd_gfx.c"
    "topbar/ui_topbar.c"
    "home/ui_home.c"
    "fonts/ui_icons.c"
    "fonts/font6x8.c"
    INCLUDE_DIRS "."
    REQUIRES driver freertos core system)
