#pragma once
#include "agui/ag_list.h"
#include "agui/ag_types.h"
#include "agui/ag_rect.h"
#include "agui/ag_painter.h"

struct __AgObj;

/**
 * @brief 虚函数表
 */
typedef struct {
    void(*draw)(struct __AgObj* obj, AgPainter* painter);
    void(*layout)(struct __AgObj* obj);
} AgObjVFunc;

/**
 * @brief 节点
 */
typedef struct __AgObj {
    /* 同层级链接 */
    AgListNode node;
    /* 父节点 */
    struct __AgObj* parent;
    /* 子节点 */
    AgList childern;
    /* 虚函数 */
    AgObjVFunc vfunc;
    struct {
        /* 需要重新绘制 */
        ag_bool redraw : 1;
        /* 无效，但不代表需要重新绘制 */
        ag_bool invalid : 1;
        /* 可见性 */
        ag_bool visiable : 1;
    } flags;
    /* 包围盒，全局独立计算免得递归更新 */
    AgRect bound;
} AgObj;

// ---------------------------------------- 基础操作 ----------------------------------------
/**
 * @brief 初始化节点
 * @param obj 
 */
void AgObj_Init(AgObj* obj);

/**
 * @brief 添加子节点，不会检查重复，不会触发绘制和布局
 * @param obj 
 * @param child 
 */
void AgObj_AddChild(AgObj* obj, AgObj* child);

/**
 * @brief 移除子节点，不会检查是否存在，不会触发绘制和布局
 * @param obj 
 * @param child 
 */
void AgObj_RemoveChild(AgObj* obj, AgObj* child);

/**
 * @brief 绘制
 * @param obj 
 * @param painter 
 */
void AgObj_DrawObj(AgObj* obj, AgPainter* painter);

// ---------------------------------------- 状态操作 ----------------------------------------
/**
 * @brief 设置是否可见
 * @param obj 
 * @param visiable 
 */
void AgObj_SetVisiable(AgObj* obj, ag_bool visiable);

/**
 * @brief 标记该节点需要重新绘制，如果被遮挡可能发生错误的绘制
 * @param obj 
 */
void AgObj_MarkRedraw(AgObj* obj);

// ---------------------------------------- 布局操作 ----------------------------------------
/**
 * @brief 设置布局
 * @param obj 
 * @param bound 
 */
void AgObj_SetBound(AgObj* obj, const AgRect* bound);

/**
 * @brief 设置位置
 * @param obj 
 * @param x 
 * @param y 
 */
void AgObj_SetPos(AgObj* obj, ag_int16 x, ag_int16 y);

/**
 * @brief 设置尺寸
 * @param obj 
 * @param w 
 * @param h 
 */
void AgObj_SetSize(AgObj* obj, ag_int16 w, ag_int16 h);
