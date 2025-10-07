#include "battery_service.h"
#include "service.h"
#include "bus.h"
#include "esp_log.h"
#include "esp_random.h"

static const char* TAG = "BatterySvc";

static void task(void* arg)
{
    (void)arg;
    uint8_t soc = 0;
    uint8_t charging = 1;
    int8_t step = 5; // change per update

    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(500));

        // Update percentage
        soc += step;

        // Reverse direction at bounds
        if (soc >= 100) {
            soc = 100;
            step = -step;
            charging = 0; // stop charging at full
        } else if (soc == 0) {
            step = -step;
            charging = 1; // start charging again
        }

        bus_msg_t m = {
            .type = MSG_EVT,
            .id = EVT_BATT_UPDATE,
            .topic = TOP_BATT,
            .len = 2,
        };
        m.payload[0] = soc;       // battery level (%)
        m.payload[1] = charging;  // 1 = charging, 0 = discharging
        bus_publish(&m);

        ESP_LOGI(TAG, "Battery: %d%%, %s", soc, charging ? "charging" : "discharging");
    }
}

void batt_service_init(void)
{
    xTaskCreatePinnedToCore(task, "BatterySvc", 2048, NULL, 5, NULL, 1);
}
