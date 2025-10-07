#include "bus.h"
#include "service.h"              // need full definitions here
#include "freertos/semphr.h"
#include "esp_timer.h"
#include <string.h>

#define BUS_MAX_SUBS 8

typedef struct {
    service_t* subs[BUS_MAX_SUBS];
    uint8_t count;
} topic_entry_t;

static topic_entry_t g_topics[TOPIC_MAX];
static SemaphoreHandle_t g_lock;

static inline uint32_t now_us(void) { return (uint32_t)esp_timer_get_time(); }

void service_bus_init(void)
{
    memset(g_topics, 0, sizeof(g_topics));
    g_lock = xSemaphoreCreateMutex();
}

bool bus_register(service_t *svc)
{
    if (!svc || svc->inbox) return false;
    svc->inbox = xQueueCreate(svc->queue_len, sizeof(bus_msg_t));
    return (svc->inbox != NULL);
}

bool bus_subscribe(service_t *svc, topic_t topic)
{
    if (!svc || topic >= TOPIC_MAX) return false;
    xSemaphoreTake(g_lock, portMAX_DELAY);
    topic_entry_t *e = &g_topics[topic];
    bool ok = false;
    if (e->count < BUS_MAX_SUBS) {
        e->subs[e->count++] = svc;
        ok = true;
    }
    xSemaphoreGive(g_lock);
    return ok;
}

bool bus_publish(const bus_msg_t *msg)
{
    if (!msg || msg->topic >= TOPIC_MAX) return false;
    topic_entry_t *e = &g_topics[msg->topic];
    bus_msg_t m = *msg; m.ts_us = now_us();

    for (int i = 0; i < e->count; i++) {
        (void)xQueueSend(e->subs[i]->inbox, &m, 0); // drop if full (non-critical)
    }
    return true;
}

bool bus_send(service_t *to, const bus_msg_t *msg)
{
    if (!to || !to->inbox || !msg) return false;
    bus_msg_t m = *msg; m.ts_us = now_us();
    return (xQueueSend(to->inbox, &m, 0) == pdTRUE);
}
