#ifndef _MESSAGE_HANDLER_H
#define _MESSAGE_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define MAX_QUEUE_SIZE              3

typedef enum {
    MSG_MODE_CHANGED,
    MSG_COLOR_CHANGED,
    MSG_DURATION_CHANGED,
    MSG_INTERVAL_CHANGED,
    MSG_SPEED_CHANGED,
    MSG_LEN_CHANGED,
    MSG_FADED_END_CHANGED,
    MSG_TAIL_LEN_CHANGED,
    MSG_HEAD_LEN_CHANGED,
    MSG_DIRECTION_CHANGED,
    MSG_EASE_CHANGED
} msg_t;

typedef struct {
    msg_t msg;
    uint8_t mode;
    union {
        struct {
            uint16_t color;
        } normal;
        struct {
            uint16_t color;
            uint16_t duration;
            uint16_t interval;
            uint8_t ease;
        } breathing;
        struct {
            uint16_t color;
            uint16_t len;
            uint16_t interval;
            uint16_t head_len;
            uint16_t tail_len;
            uint16_t speed;
            uint8_t faded_end;
            uint8_t dir;
        } lightbeam;
        struct {
            uint16_t speed;
        } rainbow;
    } setting;
} msg_struct_t;

typedef struct {
    msg_struct_t _queue[MAX_QUEUE_SIZE];
    uint8_t _size;
} queue_t;

bool queue_empty(void);
void queue_push(msg_struct_t tar);
void queue_pop(void);
msg_struct_t queue_front(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
