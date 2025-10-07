#pragma once
#include <stdbool.h>
#include "topics.h"

// forward decls (no heavy includes)
struct service_s;      typedef struct service_s service_t;
struct bus_msg_s;      typedef struct bus_msg_s bus_msg_t;

typedef struct bus_msg_s {
    msg_type_t type;
    msg_id_t   id;
    topic_t    topic;
    uint32_t   ts_us;                 // filled by bus on send
    uint8_t    len;
    uint8_t    payload[BUS_MSG_PAYLOAD];
} bus_msg_t;

// API
void service_bus_init(void);
bool bus_register(service_t *svc);                 // create inbox queue
bool bus_subscribe(service_t *svc, topic_t topic); // add subscriber to topic
bool bus_publish(const bus_msg_t *msg);            // broadcast by topic
bool bus_send(service_t *to, const bus_msg_t *msg);// direct send to a service
