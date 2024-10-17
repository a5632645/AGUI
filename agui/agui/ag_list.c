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

ag_bool AgList_IsEmpty(AgList* list) {
    return NULL == list->head && NULL == list->tail;
}

void AgList_Cut(AgList* list, AgList* ret, AgListNode* node) {
    AgList_Init(ret);
    if (!list || !ret || !node) {
        return;
    }
    /* 连接 */
    ret->tail = list->tail;
    ret->head = node;
    /* 检查 */
    if (node == list->head) {
        list->head = NULL;
        list->tail = NULL;
    }
    /* 断开连接 */
    list->tail = node->prev;
    if (NULL != node->prev) {
        node->prev->next = NULL;
    }
    node->prev = NULL;
}

void AgList_Append(AgList* list, AgList* other) {
    if (!list || !other) {
        return;
    }
    if (AgList_IsEmpty(other)) {
        return;
    }
    
    if (AgList_IsEmpty(list)) {
        list->head = other->head;
        list->tail = other->tail;
        other->head = NULL;
        other->tail = NULL;
    }
    else {
        other->head->prev = list->tail;
        list->tail->next = other->head;
        list->tail = other->tail;
        other->head = NULL;
        other->tail = NULL;
    }
}

// ---------------------------------------- AgListNode ----------------------------------------
void AgListNode_Init(AgListNode* node) {
    node->next = NULL;
    node->prev = NULL;
}