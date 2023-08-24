#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "global.h"

void initMessageHandler(void);
void sendMessage(msg_t msg, void* user_data);

extern xQueueHandle messageHandler;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
