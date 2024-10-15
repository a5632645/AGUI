#include "agui/ag_list.h"
#include "agui/ag_types.h"

// ---------------------------------------- AgList ----------------------------------------
void AgList_Init(AgList* list) {
    if (!list) {
        return;
    }

    list->head = NULL;
    list->tail = NULL;
}

void AgList_PushBack(AgList* list, AgListNode* item) {
    if (!list || !item) {
        return;
    }

    item->next = list->tail ? list->tail->next : NULL;
    item->prev = list->tail;

    if (list->tail) {
        list->tail->next = item;
    } else {
        list->head = item;
    }

    list->tail = item;
}

void AgList_PushFront(AgList* list, AgListNode* item) {
    if (!list || !item) {
        return;
    }

    item->next = list->head;
    item->prev = list->head ? list->head->prev : NULL;

    if (list->head) {
        list->head->prev = item;
    } else {
        list->tail = item;
    }

    list->head = item;
}

void AgList_Remove(AgList* list, AgListNode* item) {
    if (!list || !item) {
        return;
    }

    if (item->prev) {
        item->prev->next = item->next;
    } else {
        list->head = item->next;
    }

    if (item->next) {
        item->next->prev = item->prev;
    } else {
        list->tail = item->prev;
    }

    item->next = NULL;
    item->prev = NULL;
}

AgListNode* AgList_Popback(AgList* list) {
    if (!list || !list->tail) {
        return NULL;
    }

    AgListNode* tail = list->tail;
    AgList_Remove(list, tail);
    return tail;
}

AgListNode* AgList_Popfront(AgList* list) {
    if (!list || !list->head) {
        return NULL;
    }

    AgListNode* head = list->head;
    AgList_Remove(list, head);
    return head;
}

// ---------------------------------------- AgListNode ----------------------------------------
void AgListNode_Init(AgListNode* node) {
    node->next = NULL;
    node->prev = NULL;
}