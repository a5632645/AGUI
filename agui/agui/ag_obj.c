#include "agui/ag_obj.h"
#include "ag_comp/ag_dbg.h"
#include "ag_impl/draws_impl.h"

// ---------------------------------------- impl ----------------------------------------
static void Draw(AgObj* obj, AgPainter* painter) {
    AgFillDraw fill = {
        .color = AG_COLOR_BLACK,
    };
    AgFillDraw_Init(&fill, painter);
    AgObj_GetLocalBound(obj, &fill.rect);
    painter->call_draw(painter, &fill.draw);
}
static void Event(AgObj* obj, const AgEvent* e) {}
static void Laytout(AgObj* obj) {}

// ---------------------------------------- private ----------------------------------------
static void _InitFlags(AgObj* obj) {
    obj->flags.redraw = ag_true;
    obj->flags.invalid = ag_true;
    obj->flags.visiable = ag_true;
    obj->flags.transpant = ag_false;
}

static void _InitVFunc(AgObj* obj) {
    obj->vfunc.draw = Draw;
    obj->vfunc.layout = Laytout;
    obj->vfunc.event = Event;
}

static void _ReDraw(AgObj* obj);
static void __PopUpRedraw(AgObj* obj, const AgRect* bound) {
    if (NULL == obj) {
        return;
    }
    if (ag_true == obj->flags.redraw) {
        return;
    }
    if (ag_false == obj->flags.visiable) {
        return;
    }
    
    AgRect rect = *bound;
    rect.x += obj->bound.x;
    rect.y += obj->bound.y;
    obj->flags.invalid = ag_true;
    
    /* 同层级如果重叠也要绘制 */
    AgListNode* node = obj->node.next;
    while (NULL != node) {
        AgObj* other = AGUI_CONTAINER_OF(AgObj, node, node);
        if (ag_false == other->flags.redraw && ag_true == other->flags.visiable) {
            if (ag_true == AgRect_IsIntersected(&rect, &other->bound)) {
                if (ag_false == other->flags.transpant) {
                    __PopUpRedraw(other->parent, &other->bound);
                    other->flags.redraw = ag_true;
                    other->flags.invalid = ag_true;
                }
                else {
                    _ReDraw(other->parent);
                }
            }
        }
        node = node->next;
    }
    __PopUpRedraw(obj->parent, &rect);
}

static void _ReDraw(AgObj* obj) {
    if (NULL == obj) {
        return;
    }
    AgRect b;
    AgObj_GetLocalBound(obj, &b);
    __PopUpRedraw(obj, &b);
    obj->flags.redraw = ag_true;
    obj->flags.invalid = ag_true;
}

/**
 * @brief 递归绘制节点
 * @param obj 
 * @param painter 
 */
extern void AgDbg_DrawFrame(AgObj* obj, AgPainter* painter);
void AgObj_DrawObjInObj(AgObj* obj, AgPainter* painter) {
    if (NULL != obj) {
        /* 保存绘制区 */
        AgPainter_SaveState(painter);
        /* 移动绘制区 */
        painter->draw_aera.x += obj->bound.x;
        painter->draw_aera.y += obj->bound.y;
        painter->draw_aera.w = obj->bound.w;
        painter->draw_aera.h = obj->bound.h;

        if (ag_true == obj->flags.redraw && ag_true == obj->flags.visiable) {
            painter->always_redraw = ag_true;
        }
        
        if (ag_true == painter->always_redraw) {
            /* 如果always_redraw置位，忽视标记 */
            obj->flags.invalid = ag_false;
            if (ag_true == obj->flags.visiable) {
                obj->flags.redraw = ag_false;
                obj->vfunc.draw(obj, painter);
                /* 调试绘制边框 */
                AgDbg_DrawFrame(obj, painter);
                /* 绘制子节点 */
                AgListNode* node = obj->childern.head;
                while (NULL != node) {
                    AgObj* child = AGUI_CONTAINER_OF(AgObj, node, node);
                    AgObj_DrawObjInObj(child, painter);
                    node = node->next;
                }
            }
        }
        else {
            if (ag_true == obj->flags.invalid) {
                obj->flags.invalid = ag_false;
                if (ag_true == obj->flags.visiable) {
                    /* 绘制子节点 */
                    AgListNode* node = obj->childern.head;
                    while (NULL != node) {
                        AgObj* child = AGUI_CONTAINER_OF(AgObj, node, node);
                        AgObj_DrawObjInObj(child, painter);
                        node = node->next;
                    }
                }
            }
        }

        /* 还原绘制区 */
        AgPainter_RestoreState(painter);
    }
}

// ---------------------------------------- public ----------------------------------------
AgObj* Agobj_FirstChild(AgObj* obj) {
    if (NULL == obj) {
        return NULL;
    }
    return AGUI_CONTAINER_OF(AgObj, node, obj->childern.head);
}

AgObj* AgObj_NextSibling(AgObj* obj) {
    if (NULL == obj) {
        return NULL;
    }
    return AGUI_CONTAINER_OF(AgObj, node, obj->node.next);
}

AgObj* AgObj_PrevSibling(AgObj* obj) {
    if (NULL == obj) {
        return NULL;
    }
    return AGUI_CONTAINER_OF(AgObj, node, obj->node.prev);
}

AgObj* AgObj_LastChild(AgObj* obj) {
    if (NULL == obj) {
        return NULL;
    }
    return AGUI_CONTAINER_OF(AgObj, node, obj->childern.tail);
}

void AgObj_Init2(AgObj* obj, AgObj* parent) {
    AgListNode_Init(&obj->node);
    AgList_Init(&obj->childern);
    _InitVFunc(obj);
    _InitFlags(obj);
    AgRect_Zero(&obj->bound);
    obj->obj_type = eAgObjType_Obj;
    obj->id = 0;
    obj->parent = parent;
    if (NULL != parent) {
        AgObj_AddChild(parent, obj);
    }
}

void AgObj_Init(AgObj* obj) {
    AgObj_Init2(obj, NULL);
}

void AgObj_AddChild(AgObj* obj, AgObj* child) {
    AgList_PushBack(&obj->childern, &child->node);
    child->parent = obj;
}

void AgObj_AddChildAtBack(AgObj* obj, AgObj* child) {
    AgList_PushFront(&obj->childern, &child->node);
    child->parent = obj;
}

void AgObj_RemoveChild(AgObj* obj, AgObj* child) {
    AgList_Remove(&obj->childern, &child->node);
    child->parent = NULL;
}

void AgObj_DrawObj(AgObj* obj, AgPainter* painter) {
    AgRect rt;
    AgObj_GetLocalBound(obj, &rt);
    AgPainter_PrepareDraw(painter, &rt);

    painter->begin_frame(painter);
    AgObj_DrawObjInObj(obj, painter);
    painter->end_frame(painter);
}

void AgObj_AddChildFromArray(AgObj* obj, AgObj* childs, ag_uint32 count) {
    for (ag_uint32 i = 0; i < count; ++i) {
        AgList_PushBack(&obj->childern, &childs[i].node);
        childs[i].parent = obj;
    }
}

void AgObj_RemoveAllChild(AgObj* obj) {
    AgListNode* node = obj->childern.head;
    while (NULL != node) {
        node = AgList_Popfront(&obj->childern);
        if (NULL != node) {
            AgObj* child = AGUI_CONTAINER_OF(AgObj, node, node);
            child->parent = NULL;
        }
    }
}

void AgObj_LeaveParent(AgObj* obj) {
    if (NULL != obj->parent) {
        AgObj_RemoveChild(obj->parent, obj);
    }
}

ag_bool AgObj_HitTest(AgObj* obj, ag_int16 x, ag_int16 y) {
    if (x < obj->bound.x || x >= obj->bound.x + obj->bound.w) {
        return ag_false;
    }
    if (y < obj->bound.y || y >= obj->bound.y + obj->bound.h) {
        return ag_false;
    }
    return ag_true;
}

AgObj* AgObj_HitObj(AgObj* obj, ag_int16 x, ag_int16 y) {
    if (ag_true == AgObj_HitTest(obj, x, y)) {
        if (ag_true == AgList_IsEmpty(&obj->childern)) {
            return obj;
        }
        else {
            AgListNode* node = obj->childern.tail;
            x -= obj->bound.x;
            y -= obj->bound.y;
            while (NULL != node) {
                AgObj* child = AGUI_CONTAINER_OF(AgObj, node, node);
                if (ag_true == child->flags.visiable) {
                    AgObj* ret = AgObj_HitObj(child, x, y);
                    if (NULL != ret) {
                        return ret;
                    }
                }
                node = node->next;
            }
            return obj;
        }
    }
    return NULL;
}

void AgObj_SetVisiable(AgObj* obj, ag_bool visiable) {
    if (obj->flags.visiable == visiable) {
        return;
    }
    obj->flags.visiable = visiable;
    _ReDraw(obj->parent);
}

void AgObj_Redraw(AgObj* obj) {
    if (ag_false == obj->flags.transpant) {
        _ReDraw(obj);
    }
    else {
        _ReDraw(obj->parent);
    }
}

void AgObj_DoLayout(AgObj* obj) {
    obj->vfunc.layout(obj);
}

void AgObj_SetBound(AgObj* obj, const AgRect* bound) {
    if (ag_true == AgRect_Equal(&obj->bound, bound)) {
        return;
    }
    AgRect_Copy(&obj->bound, bound);
    obj->vfunc.layout(obj);
    _ReDraw(obj->parent);
}

void AgObj_SetBounds(AgObj* obj, ag_int16 x, ag_int16 y, ag_int16 w, ag_int16 h) {
    AgRect rect = {x, y, w, h};
    AgObj_SetBound(obj, &rect);
}

void AgObj_SetPos(AgObj* obj, ag_int16 x, ag_int16 y) {
    if (x == obj->bound.x && y == obj->bound.y) {
        return;
    }
    AgRect rect = {x, y, obj->bound.w, obj->bound.h};
    AgObj_SetBound(obj, &rect);
}

void AgObj_SetSize(AgObj* obj, ag_int16 w, ag_int16 h) {
    if (w == obj->bound.w && h == obj->bound.h) {
        return;
    }
    AgRect rect = {obj->bound.x, obj->bound.y, w, h};
    AgObj_SetBound(obj, &rect);
}

void AgObj_GetLocalBound(const AgObj* obj, AgRect* bound) {
    bound->x = 0;
    bound->y = 0;
    bound->w = obj->bound.w;
    bound->h = obj->bound.h;
}

void AgObj_BringToFront(AgObj* obj) {
    if (NULL == obj->parent) {
        return;
    }
    AgList_Remove(&obj->parent->childern, &obj->node);
    AgList_PushBack(&obj->parent->childern, &obj->node);
    _ReDraw(obj->parent);
}

void AgObj_SendToBack(AgObj* obj) {
    if (NULL == obj->parent) {
        return;
    }
    AgList_Remove(&obj->parent->childern, &obj->node);
    AgList_PushFront(&obj->parent->childern, &obj->node);
    _ReDraw(obj->parent);
}

static void _CalcSingleBound(AgObj* obj) {
    if (ag_false == AgList_IsEmpty(&obj->childern)
        && (obj->bound.w == 0 || obj->bound.h == 0)) {
        AgRect rect = {
            .x = 32767,
            .y = 32767,
            .w = -32768,
            .h = -32768
        };

        AgListNode* node = obj->childern.head;
        while (NULL != node) {
            AgObj* child = AGUI_CONTAINER_OF(AgObj, node ,node);
            rect.x = AGUI_MIN(rect.x, child->bound.x);
            rect.y = AGUI_MIN(rect.y, child->bound.y);
            rect.w = AGUI_MAX(rect.w, child->bound.x + child->bound.w);
            rect.h = AGUI_MAX(rect.h, child->bound.y + child->bound.h);
            node = node->next;
        }

        rect.w -= rect.x;
        rect.h -= rect.y;
        rect.x += obj->bound.x;
        rect.y += obj->bound.y;

        obj->bound = rect;
    }
}

void AgObj_CalcBound(AgObj* obj) {
    AgListNode* node = obj->childern.head;
    while (NULL != node) {
        AgObj* child = AGUI_CONTAINER_OF(AgObj, node, node);
        _CalcSingleBound(child);
        node = node->next;
    }
    _CalcSingleBound(obj);
}

void AgObj_SendEvent(AgObj* obj, AgEvent* event) {
    while (NULL != obj && ag_false == event->handled) {
        AgObj* parent = obj->parent;
        obj->vfunc.event(obj, event);
        obj = parent;
    }
}