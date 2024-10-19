/*
 * \file ag_mq.h
 * \brief 消息队列
*/

#pragma once
#include "agui/ag_types.h"
#include "agui/ag_list.h"

// ---------------------------------------- AgMqCallObj ----------------------------------------
typedef struct __AgMqCallObj {
    AgListNode node;
    void(*call)(void* arg);
    void* arg;
} AgMqCallObj;

/**
 * @brief 节点初始化
 * @param obj 
 */
void AgMqCallObj_Init(AgMqCallObj* obj);

// ---------------------------------------- AgMq ----------------------------------------
typedef struct __AgMq {
    AgList pending;
    AgList frees;
    void* lock;
} AgMq;

/**
 * @brief 队列初始化
 * @param mq 
 */
void AgMq_Init(AgMq* mq);

/**
 * @brief 调用所有等待的消息
 * @param mq 
 */
void AgMq_CallPending(AgMq* mq);

/**
 * @brief 入队消息
 * @param mq 
 * @param call 不可为NULL
 * @param arg 可为NULL
 * @return ag_true表示入队成功，ag_false表示内存分配失败
 */
ag_bool AgMq_Post(AgMq* mq, void(*call)(void* arg), void* arg);

/**
 * @brief 释放消息
 * @param mq 
 */
void AgMq_Destroy(AgMq* mq);