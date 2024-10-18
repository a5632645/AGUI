/*
 * \file ag_ref_obj.h
 * \brief 引用保护
*/

#pragma once
#include "agui/ag_types.h"

typedef struct {
    void* ptr;
} AgRefObj;

/**
 * @brief 初始化
 * @param obj 不能为NULL
 */
void AgRefObj_Init(AgRefObj* obj);

/**
 * @brief 摧毁一个master
 * @param obj 不能为NULL
 */
void AgRefObj_DestoryMaster(AgRefObj* obj);

/**
 * @brief master是否死亡
 * @param obj 不能为NULL
 * @return ag_true表示死亡，ag_false表示活着
 */
ag_bool AgRefObj_IsMasterDead(AgRefObj* obj);

/**
 * @brief 添加引用
 * @param obj 不能为NULL
 * @param master 不能为NULL
 */
void AgRefObj_AddRef(AgRefObj* obj, AgRefObj* master);

/**
 * @brief 摧毁slave
 * @param obj 不能为NULL
 */
void AgRefObj_DestorySlave(AgRefObj* obj);