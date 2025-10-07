#include "clock_service.h"
#include "service.h"
#include "bus.h"
#include "esp_sntp.h"
#include "esp_log.h"
#include <time.h>
#include <sys/time.h>

static const char* TAG = "ClockSvc";

static void send_tick(const struct tm* tm)
{
    bus_msg_t m = { .type = MSG_EVT, .id = EVT_TIME_TICK, .topic = TOP_TIME, .len = 2 };
    m.payload[0] = (uint8_t)tm->tm_hour;
    m.payload[1] = (uint8_t)tm->tm_min;
    bus_publish(&m);
}

static void task(void* arg)
{
    (void)arg;
    for (;;) {
        time_t now = 0; 
        time(&now);

        struct tm info; 
        localtime_r(&now, &info);   // ✅ will use Bangkok TZ after tzset()

        send_tick(&info);
        vTaskDelay(pdMS_TO_TICKS(10000)); // every 10s
    }
}

static void sntp_sync_cb(struct timeval *tv)
{
    (void)tv;
    time_t now = 0; time(&now);
    struct tm info; localtime_r(&now, &info);
    ESP_LOGI(TAG, "Time synced: %02d:%02d", info.tm_hour, info.tm_min);
}

void clock_service_init(void)
{
    // Set timezone to Bangkok (ICT = Indochina Time, GMT+7)
    setenv("TZ", "ICT-7", 1);
    tzset();

    // Init SNTP
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_set_time_sync_notification_cb(sntp_sync_cb);
    sntp_init();

    // Start the clock task
    xTaskCreatePinnedToCore(task, "ClockSvc", 3072, NULL, 5, NULL, 1);
}
