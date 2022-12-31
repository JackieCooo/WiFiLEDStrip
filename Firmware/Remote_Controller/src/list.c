#include "list.h"

static struct list_elem_t* list_back_ptr(list_t* list);
static struct list_elem_t* list_front_ptr(list_t* list);
static struct list_elem_t* list_get_ptr(list_t* list, uint8_t index);
static struct list_elem_t* list_create_elem(void* data);

list_t* list_create(void) {
    list_t* list = (list_t*) heap_caps_malloc(sizeof(list_t), MALLOC_CAP_DEFAULT);
    list->head = NULL;
    list->size = 0;
    return list;
}

bool list_empty(list_t* list) {
    return !list->head;
}

void list_push_back(list_t* list, void* data) {
    struct list_elem_t* tar = list_create_elem(data);
    struct list_elem_t* p = list_back_ptr(list);
    if (p) {
        p->next = tar;
        tar->prev = p;
    }
    else {
        list->head = tar;
    }
    ++list->size;
}

void list_push_front(list_t* list, void* data) {
    struct list_elem_t* tar = list_create_elem(data);
    struct list_elem_t* p = list_front_ptr(list);
    if (p) {
        tar->next = p;
        p->prev = tar;
    }
    list->head = tar;
    ++list->size;
}

void list_pop_back(list_t* list) {
    struct list_elem_t* p1 = list_back_ptr(list);
    if (p1) {
        struct list_elem_t* p2 = p1->prev;
        if (p2) {
            p2->next = NULL;
        }
        else {
            list->head = NULL;
        }
        heap_caps_free(p1);
    }
    --list->size;
}

void list_pop_front(list_t* list) {
    struct list_elem_t* p1 = list_front_ptr(list);
    if (p1) {
        struct list_elem_t* p2 = p1->next;
        if (p2) {
            p2->prev = NULL;
            list->head = p2;
        }
        else {
            list->head = NULL;
        }
        heap_caps_free(p1);
    }
    --list->size;
}

void* list_front(list_t* list) {
    struct list_elem_t* p = list_front_ptr(list);
    if (p) return p->data;
    return NULL;
}

void* list_back(list_t* list) {
    struct list_elem_t* p = list_back_ptr(list);
    if (p) return p->data;
    return NULL;
}

void* list_get(list_t* list, uint8_t index) {
    struct list_elem_t* p = list_get_ptr(list, index);
    if (p) return p->data;
    return NULL;
}

static struct list_elem_t* list_back_ptr(list_t* list) {
    if (!list->head) return NULL;
    struct list_elem_t *p;
    for (p = list->head; p->next; p = p->next) {}
    return p;
}

static struct list_elem_t* list_front_ptr(list_t* list) {
    return list->head;
}

static struct list_elem_t* list_get_ptr(list_t* list, uint8_t index) {
    struct list_elem_t* p = list_front_ptr(list);
    for (uint8_t i = 0; i < index && p; p = p->next, ++i) {}
    return p;
}

static struct list_elem_t* list_create_elem(void* data) {
    struct list_elem_t* elem = (struct list_elem_t*) heap_caps_malloc(sizeof(struct list_elem_t), MALLOC_CAP_DEFAULT);
    elem->data = data;
    elem->next = NULL;
    elem->prev = NULL;
    return elem;
}
