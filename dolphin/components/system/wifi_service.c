#include "wifi_service.h"
#include "service.h"
#include "bus.h"

#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include <string.h>

static const char* TAG = "WiFiSvc";

// Set your Wi-Fi here (or pass via build -D flags)
#ifndef WIFI_SSID
#define WIFI_SSID "FPT Khanh Huyen"
#endif
#ifndef WIFI_PASS
#define WIFI_PASS "06242219"
#endif

static void publish_state(uint8_t link, int8_t rssi)
{
    bus_msg_t m = { .type = MSG_EVT, .id = EVT_WIFI_STATE, .topic = TOP_WIFI, .len = 2 };
    m.payload[0] = link;
    m.payload[1] = (uint8_t)rssi;
    bus_publish(&m);
}

static void event_handler(void* arg, esp_event_base_t base, int32_t id, void* data)
{
    (void)arg; (void)base;
    if (id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (id == WIFI_EVENT_STA_DISCONNECTED) {
        publish_state(0, -127);
        esp_wifi_connect();
    } else if (id == IP_EVENT_STA_GOT_IP) {
        wifi_ap_record_t ap;
        int8_t rssi = -50;
        if (esp_wifi_sta_get_ap_info(&ap) == ESP_OK) rssi = ap.rssi;
        publish_state(1, rssi);
    }
}

void wifi_service_init(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));

    wifi_config_t wifi_cfg = { 0 };
    strncpy((char*)wifi_cfg.sta.ssid, WIFI_SSID, sizeof(wifi_cfg.sta.ssid));
    strncpy((char*)wifi_cfg.sta.password, WIFI_PASS, sizeof(wifi_cfg.sta.password));
    wifi_cfg.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_cfg));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "WiFi started, connecting to \"%s\"", WIFI_SSID);
}
