#include "agui/ag_obj.h"

// ---------------------------------------- 虚函数 ----------------------------------------
static void Draw(AgObj*, AgPainter*) {}
static void Laytout(AgObj* obj) {
    // 默认水平排列
    ag_int16 x = 0;
    AgListNode* node = obj->childern.head;
    while (NULL != node) {
        AgObj* child = AGUI_CONTAINER_OF(AgObj, node, node);
        AgObj_SetPos(child, x, 0);
        x += child->bound.w;
        node = node->next;
    }
}

// --------------------------------------------------------------------------------
static void _InitFlags(AgObj* obj) {
    obj->flags.redraw = ag_true;
    obj->flags.invalid = ag_true;
    obj->flags.visiable = ag_true;
}

static void _InitVFunc(AgObj* obj) {
    obj->vfunc.draw = Draw;
    obj->vfunc.layout = Laytout;
}

/**
 * @brief 将状态无效传递到最顶层
 * @param obj 包括该节点
 */
static void _PopInvalidToTop(AgObj* obj) {
    while (NULL != obj) {
        obj->flags.invalid = ag_true;
        obj = obj->parent;
    }
}

/**
 * @brief 标记需要绘制父节点和所有子节点
 * @param obj 
 */
static void _ReDrawParentAndAllChild(AgObj* obj) {
    obj->flags.invalid = ag_true;
    obj->flags.redraw = ag_true;
    _PopInvalidToTop(obj);
    if (obj->parent == NULL) {
        return;
    }
    /* 标记父亲和所有子节点重新绘制 */
    obj->parent->flags.invalid = ag_true;
    obj->parent->flags.redraw = ag_true;
    // AgListNode* node = obj->parent->childern.head;
    // while (NULL != node) {
    //     AgObj* child = AGUI_CONTAINER_OF(AgObj, node, node);
    //     child->flags.invalid = ag_true;
    //     child->flags.redraw = ag_true;
    //     node = node->next;
    // }
    /* 传递无效 */
}

/**
 * @brief 递归绘制节点
 * @param obj 
 * @param painter 
 */
static void _DrawObj(AgObj* obj, AgPainter* painter) {
    if (NULL != obj) {
        /* 移动绘画区域 */
        ag_int16 bck_w = painter->draw_aera.w;
        ag_int16 bck_h = painter->draw_aera.h;
        painter->draw_aera.x += obj->bound.x;
        painter->draw_aera.y += obj->bound.y;
        painter->draw_aera.w = obj->bound.w;
        painter->draw_aera.h = obj->bound.h;
        ag_bool always_draw = painter->always_redraw;
        
        if (ag_true == painter->always_redraw) {
            /* 挡该标记时，忽略无效和绘制标记，这说明该节点以下都需要绘制 */
            obj->flags.invalid = ag_false;
            if (ag_true == obj->flags.visiable) {
                /* 绘制自己 */
                obj->flags.redraw = ag_false;
                obj->vfunc.draw(obj, painter);
                /* 绘制子节点 */
                AgListNode* node = obj->childern.head;
                while (NULL != node) {
                    AgObj* child = AGUI_CONTAINER_OF(AgObj, node, node);
                    _DrawObj(child, painter);
                    node = node->next;
                }
            }
        }
        else {
            if (ag_true == obj->flags.invalid) {
                obj->flags.invalid = ag_false;
                if (ag_true == obj->flags.visiable) {
                    /* 绘制自己? */
                    if (ag_true == obj->flags.redraw) {
                        painter->always_redraw = ag_true;
                        obj->flags.redraw = ag_false;
                        obj->vfunc.draw(obj, painter);
                    }
                    /* 绘制子节点 */
                    AgListNode* node = obj->childern.head;
                    while (NULL != node) {
                        AgObj* child = AGUI_CONTAINER_OF(AgObj, node, node);
                        _DrawObj(child, painter);
                        node = node->next;
                    }
                }
            }
        }

        /* 恢复绘画区域 */
        painter->always_redraw = always_draw;
        painter->draw_aera.w = bck_w;
        painter->draw_aera.h = bck_h;
        painter->draw_aera.x -= obj->bound.x;
        painter->draw_aera.y -= obj->bound.y;
    }
}

// --------------------------------------------------------------------------------
void AgObj_Init(AgObj* obj) {
    AgListNode_Init(&obj->node);
    obj->parent = NULL;
    AgList_Init(&obj->childern);
    _InitVFunc(obj);
    _InitFlags(obj);
    AgRect_Zero(&obj->bound);
}

void AgObj_AddChild(AgObj* obj, AgObj* child) {
    AgList_PushBack(&obj->childern, &child->node);
    child->parent = obj;
}

void AgObj_RemoveChild(AgObj* obj, AgObj* child) {
    AgList_Remove(&obj->childern, &child->node);
    child->parent = NULL;
}

void AgObj_DrawObj(AgObj* obj, AgPainter* painter) {
    AgRect_Zero(&painter->draw_aera);
    painter->always_redraw = ag_false;
    painter->begin_frame(painter);
    _DrawObj(obj, painter);
    painter->end_frame(painter);
}

// --------------------------------------------------------------------------------
void AgObj_SetVisiable(AgObj* obj, ag_bool visiable) {
    if (obj->flags.visiable == visiable) {
        return;
    }
    obj->flags.visiable = visiable;
    _ReDrawParentAndAllChild(obj);
}

void AgObj_MarkRedraw(AgObj* obj) {
    // obj->flags.redraw = ag_true;
    // _PopInvalidToTop(obj);
    /* 不考虑任何重叠问题 */
    _ReDrawParentAndAllChild(obj);
}

// --------------------------------------------------------------------------------
void AgObj_SetBound(AgObj* obj, const AgRect* bound) {
    if (ag_true == AgRect_Equal(&obj->bound, bound)) {
        return;
    }
    obj->bound = *bound;
    obj->vfunc.layout(obj);
    _ReDrawParentAndAllChild(obj);
}

void AgObj_SetPos(AgObj* obj, ag_int16 x, ag_int16 y) {
    if (x == obj->bound.x && y == obj->bound.y) {
        return;
    }
    obj->bound.x = x;
    obj->bound.y = y;
    obj->vfunc.layout(obj);
    _ReDrawParentAndAllChild(obj);
}

void AgObj_SetSize(AgObj* obj, ag_int16 w, ag_int16 h) {
    if (w == obj->bound.w && h == obj->bound.h) {
        return;
    }
    obj->bound.w = w;
    obj->bound.h = h;
    obj->vfunc.layout(obj);
    _ReDrawParentAndAllChild(obj);
}