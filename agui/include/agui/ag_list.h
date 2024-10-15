#pragma once
#include "agui/ag_types.h"

typedef struct __agui_list {
    struct __agui_list* prev;
    struct __agui_list* next;
} AgListNode;

typedef struct {
    struct __agui_list* head;
    struct __agui_list* tail;
} AgList;

void AgList_Init(AgList* list);
void AgList_PushBack(AgList* list, AgListNode* item);
void AgList_PushFront(AgList* list, AgListNode* item);
void AgList_Remove(AgList* list, AgListNode* item);
AgListNode* AgList_Popback(AgList* list);
AgListNode* AgList_Popfront(AgList* list);
ag_bool AgList_IsEmpty(AgList* list);

void AgListNode_Init(AgListNode* node);