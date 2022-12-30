#include "queue.h"

static queue_t queue;

bool queue_empty(void) {
    return !queue.size;
}

void queue_push(void* tar) {
    if (queue.size < MAX_QUEUE_SIZE) {
        queue.queue[queue.size++] = tar;
    }
}

void queue_pop(void) {
    for (uint8_t i = 1; i < queue.size; ++i) {
        queue.queue[i-1] = queue.queue[i];
    }
    --queue.size;
}

void* queue_front(void) {
    return queue.queue[0];
}

