#ifndef _QUEUE_H
#define _QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <esp_heap_caps.h>

#include "list.h"

typedef list_t queue_t;

queue_t* queue_create(void);
bool queue_empty(queue_t* queue);
void queue_push(queue_t* queue, void* data);
void queue_pop(queue_t* queue);
void* queue_front(queue_t* queue);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
