#include "ui_home_service.h"
#include "service.h"
#include "bus.h"
#include "system_service.h"
#include "esp_log.h"
#include "freertos/semphr.h"
#include "home/ui_home.h"
#include "drivers/lcd_st7567.h"

#define TOPBAR_MARGIN_X   2
#define TOPBAR_MARGIN_Y   2
#define TOPBAR_HEIGHT     12

static const char* TAG = "UiHome";
static topbar_model_t g_model;
static SemaphoreHandle_t g_lock;

static void on_msg(service_t* self, const bus_msg_t* m)
{
    (void)self;
    bool isTopBarChanged = false;

    xSemaphoreTake(g_lock, portMAX_DELAY);

    switch (m->id) {
        case EVT_WIFI_STATE:
            if (m->len >= 2) {
                g_model.wifi_link = m->payload[0];
                g_model.wifi_rssi = (int8_t)m->payload[1];
                isTopBarChanged = true;
            }
            break;
        case EVT_BATT_UPDATE:
            if (m->len >= 2) {
                g_model.batt_soc = m->payload[0];
                g_model.charging = m->payload[1];
                isTopBarChanged = true;
            }
            break;
        case EVT_TIME_TICK:
            if (m->len >= 2) {
                g_model.hour = m->payload[0];
                g_model.minute = m->payload[1];
                isTopBarChanged = true;
            }
            break;
        case EVT_NOTIFY_SUMMARY:
            if (m->len >= 1) {
                g_model.notif_count = m->payload[0];
                isTopBarChanged = true;
            }
            break;
        default: break;
    }
    xSemaphoreGive(g_lock);

    if (isTopBarChanged) ui_render_topbar(&g_model);
    ui_render_home();
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

static service_t g_ui_home = {
    .name = "UiHome",
    .on_msg = on_msg,
    .priority = 5,
    .core = 1,
    .queue_len = 8,
};

void ui_home_service_init(void)
{
    bus_register(&g_ui_home);
    bus_subscribe(&g_ui_home, TOP_UI);
    bus_subscribe(&g_ui_home, TOP_WIFI);
    bus_subscribe(&g_ui_home, TOP_TIME);
    bus_subscribe(&g_ui_home, TOP_NOTIFY);
    xTaskCreatePinnedToCore(
        svc_task,
        g_ui_home.name,
        4096,
        &g_ui_home,
        g_ui_home.priority,
        &g_ui_home.task,
        g_ui_home.core
    );

    ESP_LOGI(TAG, "Home UI service initialized");
}
