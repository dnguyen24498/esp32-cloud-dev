#pragma once
#include <stdint.h>

typedef enum {
    TOP_SYS = 0,
    TOP_WIFI,
    TOP_BT,
    TOP_BATT,
    TOP_TIME,
    TOP_NOTIFY,
    TOP_UI,
    TOP_RSRC,
    TOPIC_MAX
} topic_t;

typedef enum { MSG_EVT, MSG_CMD } msg_type_t;

typedef enum {
    // events
    EVT_WIFI_STATE = 1,
    EVT_BT_STATE,
    EVT_BATT_UPDATE,
    EVT_TIME_TICK,
    EVT_NOTIFY_SUMMARY,
    EVT_RSRC_SNAPSHOT,
    // commands
    CMD_THROTTLE = 100,
    CMD_PAUSE,
    CMD_KILL,
    CMD_UI_RENDER_TICK,
    CMD_REFRESH_STATE
} msg_id_t;

typedef struct {
    uint8_t  hour, minute;
    int8_t   wifi_rssi;
    uint8_t  wifi_link;     // 0/1
    uint8_t  bt_on, bt_conn;
    uint8_t  batt_soc;      // 0..100
    uint8_t  charging;      // 0/1
    uint8_t  notif_count;
} topbar_model_t;

#define BUS_MSG_PAYLOAD  32
