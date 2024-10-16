/*
 * @file ag_key.h
 * @brief 用于无触摸屏的纯按键输入切换器
*/

#pragma once
#include "agui/ag_obj.h"

typedef struct {
    AgObj* root;
    AgObj* current;
    AgObj* highlight;
} AgKeySwitcher;

/**
 * @brief 初始化
 * @param ks 
 * @param root 
 * @param highlight 高亮组件
 */
void AgKeySwitcher_Init(AgKeySwitcher* ks, AgObj* root, AgObj* highlight);

/**
 * @brief 设置当前物体
 * @param ks 
 * @param obj 
 */
void AgKeySwitcher_SetCurrent(AgKeySwitcher* ks, AgObj* obj);

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