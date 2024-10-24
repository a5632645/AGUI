/*
 * \file ag_ref_obj.h
 * \brief 引用保护
*/

#pragma once
#include "agui/ag_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void* ptr;
} AgMasterRefObj;

typedef AgMasterRefObj AgSlaveRefObj;

/**
 * @brief 初始化
 * @param obj 不能为NULL
 */
void AgMasterRefObj_Init(AgMasterRefObj* obj);

/**
 * @brief 摧毁一个master
 * @param obj 不能为NULL
 */
void AgMasterRefObj_Destory(AgMasterRefObj* obj);

/**
 * @brief 添加引用
 * @param obj 不能为NULL
 * @param master 不能为NULL
 */
void AgSlaveRefObj_Init(AgSlaveRefObj* obj, AgMasterRefObj* master);

/**
 * @brief master是否死亡
 * @param obj 不能为NULL
 * @return ag_true表示死亡，ag_false表示活着
 */
ag_bool AgSlaveRefObj_IsMasterDead(AgSlaveRefObj* obj);

/**
 * @brief 摧毁slave
 * @param obj 不能为NULL
 */
void AgSlaveRefObj_Destory(AgSlaveRefObj* obj);

#ifdef __cplusplus
}
#endif