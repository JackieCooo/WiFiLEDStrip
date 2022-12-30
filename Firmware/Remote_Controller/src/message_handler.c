#include "message_handler.h"

bool msg_aviliable(void) {
    return !queue_empty();
}

void msg_send(msg_t type, void* user_data) {
    msg_struct_t* data = (msg_struct_t*) heap_caps_malloc(sizeof(msg_struct_t), MALLOC_CAP_DEFAULT);
    data->msg = type;
    memcpy(data->user_data, user_data, sizeof(user_data));
    queue_push((void*) data);
}

msg_struct_t msg_receive(void) {
    msg_struct_t res;

    msg_struct_t* data = (msg_struct_t*) queue_front();
    memcpy(&res, data, sizeof(data));
    queue_pop();
    heap_caps_free(data);
    data = NULL;

    return res;
}
