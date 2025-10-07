idf_component_register(SRCS 
                      "wifi_service.c" 
                      "ui_topbar_service.c" 
                      "ui_home_service.c" 
                      "clock_service.c" 
                      "battery_service.c" 
                      "system_service.c" 
                      "resource_mgr.c"
                      INCLUDE_DIRS "."
                      REQUIRES core esp_wifi esp_event esp_netif lwip nvs_flash ui freertos esp_timer esp_system)