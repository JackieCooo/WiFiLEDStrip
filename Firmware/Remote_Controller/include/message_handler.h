#ifndef _MESSAGE_HANDLER_H
#define _MESSAGE_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <esp_heap_caps.h>
#include <string.h>

#include "global.h"
#include "queue.h"

typedef enum {
    MSG_MODE_CHANGED,
    MSG_COLOR_CHANGED,
    MSG_UPDATE_CONFIG,
    MSG_MATCH,
} msg_t;

typedef struct {
    msg_t msg;
    void* user_data;
} msg_struct_t;

typedef struct {
    msg_struct_t data;
    bool resp;
} msg_reply_t;

bool msg_aviliable(void);
void msg_send(msg_t type, void* user_data);
msg_struct_t msg_receive(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
