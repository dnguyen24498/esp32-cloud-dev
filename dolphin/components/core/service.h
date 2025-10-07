#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "bus.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct service_s service_t;
typedef void (*svc_msg_cb)(service_t*, const bus_msg_t*);

struct service_s {
    const char*   name;
    QueueHandle_t inbox;
    TaskHandle_t  task;
    svc_msg_cb    on_msg;
    UBaseType_t   priority;
    BaseType_t    core;
    UBaseType_t   queue_len;
};
