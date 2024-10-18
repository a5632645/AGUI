/*
 * @file ag_key.h
 * @brief 用于无触摸屏的纯按键输入切换器
*/

#pragma once
#include "agui/ag_obj.h"
#include "agui/ag_types.h"

typedef enum {
    eAgKeyAction_GoDown = 0,
    eAgKeyAction_GoUp,
    eAgKeyAction_GoNext,
    eAgKeyAction_GoPrev,
    eAgKeyAction_GoRoot,
} AgKeySwitActionEnum;

typedef struct __AgKeySwitcher {
    AgObj* root;
    AgObj* current;
    /* 是否选择该obj, 返回值为ag_true表示该obj能被选中 */
    ag_bool(*filter)(AgObj* obj);
    /* obj被选中时的回调 */
    void(*obj_selected)(AgObj* obj);
    /* 事件调度 */
    void(*event)(struct __AgKeySwitcher* ks, AgEvent* event);
} AgKeySwitcher;

/**
 * @brief 初始化
 * @param ks 
 * @param root 
 * @param highlight 高亮组件
 */
void AgKeySwitcher_Init(AgKeySwitcher* ks, AgObj* root);

/**
 * @brief 设置当前物体
 * @param ks 
 * @param obj 不能为NULL
 */
void AgKeySwitcher_Goto(AgKeySwitcher* ks, AgObj* obj);

/**
 * @brief 深入到当前节点的下一层
 * @param ks 
 */
void AgKeySwitcher_GoDown(AgKeySwitcher* ks);

/**
 * @brief 回到当前节点的上一层
 * @param ks 
 */
void AgKeySwitcher_GoUp(AgKeySwitcher* ks);

/**
 * @brief 移动到同级节点下一个
 * @param ks 
 */
void AgKeySwitcher_GoNext(AgKeySwitcher* ks);

/**
 * @brief 移动到同级节点上一个
 * @param ks 
 */
void AgKeySwitcher_GoPrev(AgKeySwitcher* ks);

/**
 * @brief 回到跟节点
 * @param ks 
 */
void AgKeySwitcher_GoRoot(AgKeySwitcher* ks);

/**
 * @brief 发送事件，如果switcher未处理，将转发到current obj(sender会被改写)
 * @param ks 
 * @param event 不能为NULL
 */
void AgKeySwitcher_SendEvent(AgKeySwitcher* ks, AgEvent* event);