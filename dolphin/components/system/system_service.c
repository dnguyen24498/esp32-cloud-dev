#include "system_service.h"
#include "service.h"
#include "bus.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include <string.h>

static const char* TAG = "SystemService";
static SemaphoreHandle_t g_lock;

static void on_msg(service_t* self, const bus_msg_t* m)
{
    (void)self;
}

static void svc_task(void* arg)
{
    service_t* svc = (service_t*)arg;
    bus_msg_t m;
    for (;;) {
        if (xQueueReceive(svc->inbox, &m, portMAX_DELAY)) {
            svc->on_msg(svc, &m);
        }
    }
}

static service_t g_sys = {
    .name = "SystemService",
    .on_msg = on_msg,
    .priority = 9,
    .core = 0,
    .queue_len = 16,
};

void system_service_init(void)
{
    g_lock = xSemaphoreCreateMutex();
    bus_register(&g_sys);
    bus_subscribe(&g_sys, TOP_WIFI);
    bus_subscribe(&g_sys, TOP_BATT);
    bus_subscribe(&g_sys, TOP_TIME);
    bus_subscribe(&g_sys, TOP_NOTIFY);
    xTaskCreatePinnedToCore(svc_task, g_sys.name, 4096, &g_sys,
                            g_sys.priority, &g_sys.task, g_sys.core);
    ESP_LOGI(TAG, "started");
}
