#include "queue.h"

queue_t* queue_create(void) {
    queue_t* queue = (queue_t*) heap_caps_malloc(sizeof(queue_t), MALLOC_CAP_DEFAULT);
    queue = list_create();
    return queue;
}

bool queue_empty(queue_t* queue) {
    return list_empty(queue);
}

void queue_push(queue_t* queue, void* data) {
    list_push_back(queue, data);
}

void queue_pop(queue_t* queue) {
    list_pop_front(queue);
}

void* queue_front(queue_t* queue) {
    return list_front(queue);
}
