idf_component_register(SRCS "dolphin.c"
                      INCLUDE_DIRS "."
                      REQUIRES core system driver services ui nvs_flash esp_wifi esp_event)
