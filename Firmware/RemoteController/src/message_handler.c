#include "message_handler.h"

xQueueHandle messageHandler;

void initMessageHandler(void) {
    messageHandler = xQueueCreate(3, sizeof(msg_request_t));
}

void sendMessage(msg_t msg, void* user_data) {
    msg_request_t request;
    request.msg = msg;
    request.user_data = user_data;
    xQueueSend(messageHandler, &request, QUEUE_TIMEOUT_MS);
}
