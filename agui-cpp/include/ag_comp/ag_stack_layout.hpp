/*
 * \file ag_stack_layout.h
 * \brief 可堆叠的布局，出栈入栈
*/

#pragma once
#include "ag_comp/deps.h"

typedef struct {
    AgObj obj;
    AgList stack;
} AgStackLayout;

/**
 * @brief 初始化
 * @param sl 不能为NULL
 */
void AgStackLayout_Init(AgStackLayout* sl);

/**
 * @brief 入栈
 * @param sl 不能为NULL
 * @param obj 不能与栈中或者正在显示的节点为同一个实例
 */
void AgStackLayout_Push(AgStackLayout* sl, AgObj* obj);

/**
 * @brief 入栈，如果栈中已经存在该实例，则将其取出作为使用
 * @param sl 不能为NULL
 * @param obj 可以为NULL
 */
void AgStackLayout_Push2(AgStackLayout* sl, AgObj* obj);

/**
 * @brief 入栈，如果栈中已经存在该实例，弹出其上方的所有节点
 * @param sl 不能为NULL
 * @param ret 返回的被弹出的节点，可以不初始化，不能为NULL
 * @param obj 可以为NULL
 */
void AgStackLayout_Push3(AgStackLayout* sl, AgList* ret, AgObj* obj);

/**
 * @brief 出栈
 * @param sl 不能为NULL
 * @return 可能为NULL
 */
AgObj* AgStackLayout_Pop(AgStackLayout* sl);

/**
 * @brief 获取正在显示的节点
 * @param sl 不能为NULL
 * @return 可能为NULL
 */
AgObj* AgStackLayout_Current(AgStackLayout* sl);