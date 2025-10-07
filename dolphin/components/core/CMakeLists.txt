idf_component_register(SRCS "service.c" "bus.c"
                      INCLUDE_DIRS "."
                      REQUIRES freertos esp_timer)
