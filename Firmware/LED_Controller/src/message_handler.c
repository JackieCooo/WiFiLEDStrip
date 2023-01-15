#include "message_handler.h"

static queue_t* queue;

void msg_init(void) {
    queue = queue_create();
}

bool msg_aviliable(void) {
    return !queue_empty(queue);
}

void msg_send(msg_t type, void* user_data) {
    msg_request_t* data = (msg_request_t*) heap_caps_malloc(sizeof(msg_request_t), MALLOC_CAP_DEFAULT);
    data->msg = type;
    data->user_data = user_data;
    queue_push(queue, data);
}

msg_request_t msg_receive(void) {
    msg_request_t res;

    msg_request_t* data = (msg_request_t*) queue_front(queue);
    memcpy(&res, data, sizeof(msg_request_t));
    queue_pop(queue);
    heap_caps_free(data);
    data = NULL;

    return res;
}

const msg_request_t* msg_peek(void) {
    void* data = queue_front(queue);
    if (data) return (const msg_request_t*) data;
    return NULL;
}
