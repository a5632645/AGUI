#pragma once
#include "agui/ag_list.h"
#include "agui/ag_types.h"
#include "agui/ag_rect.h"
#include "agui/ag_painter.h"
#include "agui/ag_event.h"

struct __AgObj;

/**
 * @brief 虚函数表
 */
typedef struct {
    /* 绘制，空间坐标按实现给，超出父节点范围的节点也会被绘制 */
    void(*draw)(struct __AgObj* obj, AgPainter* painter);
    /* 布局，obj是父节点 */
    void(*layout)(struct __AgObj* obj);
    /* 事件 */
    void(*event)(struct __AgObj* obj, const AgEvent* event);
} AgObjVFunc;

enum {
    eAgObjType_Obj = 0
};

/**
 * @brief 节点，默认填充黑色
 */
typedef struct __AgObj {
    /* 同层级链接 */
    AgListNode node;
    /* 节点类型,0=AgObj */
    ag_uint16 obj_type;
    /* 节点ID，0未分配 */
    ag_uint16 id;
    /* 父节点 */
    struct __AgObj* parent;
    /* 子节点，注意不会释放内存 */
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
        /* 是否是半透明 */
        ag_bool transpant : 1;
    } flags;
    /* 在父节点空间的位置，全局独立计算免得递归更新 */
    AgRect bound;
} AgObj;

// ---------------------------------------- 索引操作 ----------------------------------------
/**
 * @brief 该节点的第一个子节点
 * @param obj 可以为NULL
 * @return 可能为NULL
 */
AgObj* Agobj_FirstChild(AgObj* obj);

/**
 * @brief 该节点的下一个同层级节点
 * @param obj 可以为NULL
 * @return 可能为NULL
 */
AgObj* AgObj_NextSibling(AgObj* obj);

/**
 * @brief 该节点的上一个同层级节点
 * @param obj 可以为NULL
 * @return 可能为NULL
 */
AgObj* AgObj_PrevSibling(AgObj* obj);

/**
 * @brief 该节点的最后一个子节点
 * @param obj 可以为NULL
 * @return 可能为NULL
 */
AgObj* AgObj_LastChild(AgObj* obj);

// ---------------------------------------- 基础操作 ----------------------------------------
/**
 * @brief 初始化节点
 * @param obj 不能为NULL
 * @param parent 可以为NULL
 */
void AgObj_Init2(AgObj* obj, AgObj* parent);

/**
 * @brief 初始化节点
 * @param obj 不能为NULL
 */
void AgObj_Init(AgObj* obj);

/**
 * @brief 添加子节点，不会检查重复，不会触发绘制和布局
 * @param obj 
 * @param child 
 */
void AgObj_AddChild(AgObj* obj, AgObj* child);

/**
 * @brief 添加子节点，不会检查重复，不会触发绘制和布局，处于最底层
 * @param obj 
 * @param child 
 */
void AgObj_AddChildAtBack(AgObj* obj, AgObj* child);

/**
 * @brief 从某个数组中添加子节点，不会触发绘制和布局
 * @param obj 
 * @param childs 
 * @param count 
 */
void AgObj_AddChildFromArray(AgObj* obj, AgObj* childs, ag_uint32 count);

/**
 * @brief 移除子节点，不会检查是否存在，不会触发绘制和布局
 * @param obj 
 * @param child 
 */
void AgObj_RemoveChild(AgObj* obj, AgObj* child);

/**
 * @brief 移除所有的子节点，不会触发绘制和布局
 * @param obj 
 */
void AgObj_RemoveAllChild(AgObj* obj);

/**
 * @brief 从父节点移除
 * @param obj 
 */
void AgObj_LeaveParent(AgObj* obj);

/**
 * @brief 绘制obj，不能在obj.vfun.draw里调用
 * @param obj 不能为NULL
 * @param painter 不能为NULL
 */
void AgObj_DrawObj(AgObj* obj, AgPainter* painter);

/**
 * @brief 绘制obj，只能在obj.vfun.draw里调用
 * @param obj 可以为NULL
 * @param painter 不能为NULL
 */
void AgObj_DrawObjInObj(AgObj* obj, AgPainter* painter);

/**
 * @brief 测试是否被点击
 * @param obj 
 * @param x 
 * @param y 
 * @return 
 */
ag_bool AgObj_HitTest(AgObj* obj, ag_int16 x, ag_int16 y);

/**
 * @brief 找到被点击的节点
 * @param obj 
 * @param x 
 * @param y 
 * @return 
 */
AgObj* AgObj_HitObj(AgObj* obj, ag_int16 x, ag_int16 y);

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
void AgObj_Redraw(AgObj* obj);

// ---------------------------------------- 布局操作 ----------------------------------------
/**
 * @brief 触发布局
 * @param obj 
 */
void AgObj_DoLayout(AgObj* obj);

/**
 * @brief 设置布局
 * @param obj 
 * @param bound 
 */
void AgObj_SetBound(AgObj* obj, const AgRect* bound);

/**
 * @brief 设置布局
 * @param obj 
 * @param x 
 * @param y 
 * @param w 
 * @param h 
 */
void AgObj_SetBounds(AgObj* obj, ag_int16 x, ag_int16 y, ag_int16 w, ag_int16 h);

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

/**
 * @brief 获取边界在本地空间下的大小
 * @param obj 
 * @param bound 
 */
void AgObj_GetLocalBound(AgObj* obj, AgRect* bound);

// ---------------------------------------- z操作 ----------------------------------------
/**
 * @brief 将节点置顶
 * @param obj 
 */
void AgObj_BringToFront(AgObj* obj);

/**
 * @brief 将节点放到最底层
 * @param obj 
 */
void AgObj_SendToBack(AgObj* obj);

// ---------------------------------------- 奇怪的操作 ----------------------------------------
/**
 * @brief 计算节点的边界，大小为0会被计算
 * @param obj 
 */
void AgObj_CalcBound(AgObj* obj);

/**
 * @brief 发送事件，如果不处理将发送到父节点
 * @param obj 可为NULL
 * @param event 不能为NULL
 */
void AgObj_SendEvent(AgObj* obj, AgEvent* event);