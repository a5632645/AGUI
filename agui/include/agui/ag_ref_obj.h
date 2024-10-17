/*
 * \file ag_ref_obj.h
 * \brief 引用保护
*/

#pragma once
#include "agui/ag_list.h"
#include "agui/ag_types.h"
#include "ag_impl/atomic_impl.h"

// ---------------------------------------- AgRefObj ----------------------------------------
typedef struct {
    AgListNode node;
    ag_atomic_bool_t master_dead;
} AgRefObj;

/**
 * @brief 判断master是否销毁
 * @param obj 最好不为NULL
 * @return ag_true为销毁或者NULL
 */
ag_bool AgRefObj_IsMasterDead(AgRefObj* obj);

// ---------------------------------------- AgMasterRefObj ----------------------------------------
typedef struct {
    AgList refs;
    ag_bool dead;
    void* lock;
} AgMasterRefObj;

/**
 * @brief 初始化一个master
 * @param obj 可为NULL
 */
void AgMasterRefObj_Init(AgMasterRefObj* obj);

/**
 * @brief 添加一个引用
 * @param obj 可为NULL
 * @param ref 可不初始化
 */
void AgMasterRefObj_AddRef(AgMasterRefObj* obj, AgRefObj* ref);

/**
 * @brief 移除一个引用
 * @param obj 可为NULL
 * @param ref 可为NULL
 */
void AgMasterRefObj_Remove(AgMasterRefObj* obj, AgRefObj* ref);

/**
 * @brief 摧毁一个master
 * @param obj 可为NULL
 */
void AgMasterRefObj_Destroy(AgMasterRefObj* obj);