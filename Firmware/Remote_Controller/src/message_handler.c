#include "message_handler.h"

static queue_t queue;

bool queue_empty(void) {
    return !queue._size;
}

void queue_push(msg_struct_t tar) {
    if (queue._size < MAX_QUEUE_SIZE) {
        queue._queue[queue._size++] = tar;
    }
}

void queue_pop(void) {
    for (uint8_t i = 1; i < queue._size; ++i) {
        queue._queue[i-1] = queue._queue[i];
    }
    --queue._size;
}

msg_struct_t queue_front(void) {
    return queue._queue[0];
}
