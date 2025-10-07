#include "resource_mgr.h"
#include "service.h"
#include "bus.h"
#include "esp_system.h"
#include "esp_log.h"
#include <string.h>

static const char* TAG = "ResourceMgr";

static void rsrc_task(void* arg)
{
    (void)arg;
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(1000));
        uint32_t free_heap = esp_get_free_heap_size();

        bus_msg_t evt = {
            .type = MSG_EVT, .id = EVT_RSRC_SNAPSHOT, .topic = TOP_RSRC, .len = 4
        };
        memcpy(evt.payload, &free_heap, 4);
        bus_publish(&evt);

        if (free_heap < 25*1024) {
            bus_msg_t cmd = { .type = MSG_CMD, .id = CMD_THROTTLE, .topic = TOP_SYS, .len = 1 };
            cmd.payload[0] = 2;
            bus_publish(&cmd);
            ESP_LOGW(TAG, "Low heap %u bytes, throttling L2", (unsigned)free_heap);
        }
    }
}

void rsrc_service_init(void)
{
    xTaskCreatePinnedToCore(rsrc_task, "ResourceMgr", 4096, NULL, 10, NULL, 0);
}
