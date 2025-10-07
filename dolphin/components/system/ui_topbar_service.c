#include "ui_topbar_service.h"
#include "service.h"
#include "bus.h"
#include "system_service.h"
#include "esp_log.h"
#include "freertos/semphr.h"
#include "topbar/ui_topbar.h"
#include "drivers/lcd_st7567.h"

static const char* TAG = "UiTopBar";
static topbar_model_t g_model;
static SemaphoreHandle_t g_lock;

static void on_msg(service_t* self, const bus_msg_t* m)
{
    (void)self;
    bool changed = false;

    xSemaphoreTake(g_lock, portMAX_DELAY);
    switch (m->id) {
        case EVT_WIFI_STATE:
            if (m->len >= 2) {
                g_model.wifi_link = m->payload[0];
                g_model.wifi_rssi = (int8_t)m->payload[1];
                changed = true;
            }
            break;
        case EVT_BATT_UPDATE:
            if (m->len >= 2) {
                g_model.batt_soc = m->payload[0];
                g_model.charging = m->payload[1];
                changed = true;
            }
            break;
        case EVT_TIME_TICK:
            if (m->len >= 2) {
                g_model.hour    = m->payload[0];
                g_model.minute  = m->payload[1];
                changed = true;
            }
            break;
        case EVT_NOTIFY_SUMMARY:
            if (m->len >= 1) {
                g_model.notif_count = m->payload[0];
                changed = true;
            }
            break;
        default: break;
    }
    xSemaphoreGive(g_lock);

    if (changed) ui_render_topbar(&g_model);
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

static service_t g_ui = {
    .name = "UiTopBar",
    .on_msg = on_msg,
    .priority = 6,
    .core = 1,
    .queue_len = 8,
};

void ui_topbar_service_init(void)
{
    bus_register(&g_ui);
    bus_subscribe(&g_ui, TOP_UI);
    bus_subscribe(&g_ui, TOP_WIFI);
    bus_subscribe(&g_ui, TOP_TIME);
    bus_subscribe(&g_ui, TOP_NOTIFY);
    xTaskCreatePinnedToCore(svc_task, g_ui.name, 4096, &g_ui,
                            g_ui.priority, &g_ui.task, g_ui.core);
}
