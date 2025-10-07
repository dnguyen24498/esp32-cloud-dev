#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "service.h"
#include "bus.h"

#include "system_service.h"
#include "resource_mgr.h"

#include "ui_topbar_service.h"
#include "ui_home_service.h"
#include "wifi_service.h"
#include "battery_service.h"
#include "clock_service.h"
#include "drivers/lcd_st7567.h"

static const char* TAG = "APP";

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    // Init display
    lcd_i2c_init(21, 22, 100000);
    lcd_init(0xA3, 0x23, 0x20);

    service_bus_init(); // init pub/sub

    system_service_init();   // aggregator/topbar model
    rsrc_service_init();     // resource monitoring/policies
    wifi_service_init();     // Wi-Fi STA (publishes EVT_WIFI_STATE)
    batt_service_init();     // battery mock (publishes EVT_BATT_UPDATE)
    clock_service_init();    // SNTP + minute/10s ticks
    ui_topbar_service_init();       // logs the top bar
    ui_home_service_init();

    ESP_LOGI(TAG, "System boot complete.");
}
