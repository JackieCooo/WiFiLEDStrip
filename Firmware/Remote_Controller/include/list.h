#ifndef LVGL_LIST_H
#define LVGL_LIST_H

#include <stdint.h>
#include <stdbool.h>
#include <malloc.h>

#ifdef __cplusplus
extern "C" {
#endif

struct list_elem_t {
    void* data;
    struct list_elem_t* next;
    struct list_elem_t* prev;
};

typedef struct {
    struct list_elem_t* head;
    uint8_t size;
} list_t;

list_t* list_create(void);
bool list_empty(list_t* list);
void list_push_back(list_t* list, void* data);
void list_push_front(list_t* list, void* data);
void list_pop_back(list_t* list);
void list_pop_front(list_t* list);
void list_clear(list_t* list);
void list_del(list_t* list);
void* list_front(list_t* list);
void* list_back(list_t* list);
void* list_get(list_t* list, uint8_t index);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //LVGL_LIST_H
