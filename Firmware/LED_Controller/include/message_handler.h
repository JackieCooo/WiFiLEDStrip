#ifndef _MESSAGE_HANDLER_H
#define _MESSAGE_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <esp_heap_caps.h>
#include <string.h>

#include "queue.h"

typedef enum {
    MSG_READ_CONFIG,
    MSG_WRITE_CONFIG,
    MSG_MATCH,
} msg_t;

typedef struct {
    msg_t msg;
    void* user_data;
} msg_request_t;

typedef struct {
    msg_request_t data;
    bool resp;
} msg_reply_t;

void msg_init(void);
bool msg_aviliable(void);
void msg_send(msg_t type, void* user_data);
msg_request_t msg_receive(void);
const msg_request_t* msg_peek(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
