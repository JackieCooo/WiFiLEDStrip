#ifndef _QUEUE_H
#define _QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define MAX_QUEUE_SIZE              3

typedef struct {
    void* queue[MAX_QUEUE_SIZE];
    uint8_t size;
} queue_t;

bool queue_empty(void);
void queue_push(void* tar);
void queue_pop(void);
void* queue_front(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
