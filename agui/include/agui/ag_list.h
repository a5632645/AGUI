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

/**
 * @brief 初始化链表
 * @param list 
 */
void AgList_Init(AgList* list);

/**
 * @brief 在末尾插入节点
 * @param list 
 * @param item 
 */
void AgList_PushBack(AgList* list, AgListNode* item);

/**
 * @brief 在头部插入节点
 * @param list 
 * @param item 
 */
void AgList_PushFront(AgList* list, AgListNode* item);

/**
 * @brief 移除节点
 * @param list 
 * @param item 
 */
void AgList_Remove(AgList* list, AgListNode* item);

/**
 * @brief 从末尾移除节点
 * @param list 
 * @return 
 */
AgListNode* AgList_Popback(AgList* list);

/**
 * @brief 从头部移除节点
 * @param list 
 * @return 
 */
AgListNode* AgList_Popfront(AgList* list);

/**
 * @brief 链表是否为空
 * @param list 
 * @return 
 */
ag_bool AgList_IsEmpty(AgList* list);

/**
 * @brief 从某个节点截断链表到新的链表
 * @param list 
 * @param ret 可不初始化
 * @param node 新的头部节点
 */
void AgList_Cut(AgList* list, AgList* ret, AgListNode* node);

/**
 * @brief 连接两个链表
 * @param list 不可为NULL
 * @param other 可为NULL
 */
void AgList_Append(AgList* list, AgList* other);

/**
 * @brief 初始化节点
 * @param node 
 */
void AgListNode_Init(AgListNode* node);
