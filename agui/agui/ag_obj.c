#include "agui/ag_obj.h"
#include "ag_comp/ag_dbg.h"

// ---------------------------------------- 铏氬嚱鏁?----------------------------------------
#if AGUI_DBG_WRIE_FRAME == 0
static void _AgObj_Draw(AgObj* obj, AgPainter* painter) {}
#else
extern void _AgObj_Draw(AgObj* obj, AgPainter* painter);
#endif
static void Event(AgObj* obj, const AgEvent* e) {}
static void Laytout(AgObj* obj) {}

// --------------------------------------------------------------------------------
static void _InitFlags(AgObj* obj) {
    obj->flags.redraw = ag_true;
    obj->flags.invalid = ag_true;
    obj->flags.visiable = ag_true;
    obj->flags.transpant = ag_false;
}

static void _InitVFunc(AgObj* obj) {
    obj->vfunc.draw = _AgObj_Draw;
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
    
    /* 鍚屽眰绾ф爣璁皉edraw */
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
 * @brief 閫掑綊缁樺埗鑺傜偣锛岃秴鍑鸿寖鍥寸殑鑺傜偣涔熶細琚粯鍒讹紒
 * @param obj 
 * @param painter 
 */
static void _DrawObj(AgObj* obj, AgPainter* painter) {
    if (NULL != obj) {
        /* 淇濆瓨缁樼敾鍖哄煙 */
        ag_int16 bck_w = painter->draw_aera.w;
        ag_int16 bck_h = painter->draw_aera.h;
        ag_bool always_draw = painter->always_redraw;
        /* 绉诲姩缁樼敾鍖哄煙 */
        painter->draw_aera.x += obj->bound.x;
        painter->draw_aera.y += obj->bound.y;
        painter->draw_aera.w = obj->bound.w;
        painter->draw_aera.h = obj->bound.h;

        if (ag_true == obj->flags.redraw && ag_true == obj->flags.visiable) {
            painter->always_redraw = ag_true;
        }
        
        if (ag_true == painter->always_redraw) {
            /* 鎸¤鏍囪鏃讹紝蹇界暐鏃犳晥鍜岀粯鍒舵爣璁帮紝杩欒鏄庤鑺傜偣浠ヤ笅閮介渶瑕佺粯鍒?*/
            obj->flags.invalid = ag_false;
            if (ag_true == obj->flags.visiable) {
                /* 缁樺埗鑷繁 */
                obj->flags.redraw = ag_false;
                obj->vfunc.draw(obj, painter);
                /* 缁樺埗瀛愯妭鐐?*/
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
                    /* 缁樺埗瀛愯妭鐐?*/
                    AgListNode* node = obj->childern.head;
                    while (NULL != node) {
                        AgObj* child = AGUI_CONTAINER_OF(AgObj, node, node);
                        _DrawObj(child, painter);
                        node = node->next;
                    }
                }
            }
        }

        /* 鎭㈠缁樼敾鍖哄煙 */
        painter->always_redraw = always_draw;
        painter->draw_aera.w = bck_w;
        painter->draw_aera.h = bck_h;
        painter->draw_aera.x -= obj->bound.x;
        painter->draw_aera.y -= obj->bound.y;
    }
}

// ---------------------------------------- 鍒濆鍖?----------------------------------------
void AgObj_Init(AgObj* obj) {
    AgListNode_Init(&obj->node);
    obj->parent = NULL;
    AgList_Init(&obj->childern);
    _InitVFunc(obj);
    _InitFlags(obj);
    AgRect_Zero(&obj->bound);

    obj->obj_type = eAgObjType_Obj;
    obj->id = 0;
}

// ---------------------------------------- 瀛愯妭鐐?----------------------------------------
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
    AgObj_GetLocalBound(obj, &painter->draw_aera);
    painter->always_redraw = ag_false;
    painter->begin_frame(painter);
    _DrawObj(obj, painter);
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

// ---------------------------------------- 鐘舵€佹搷浣?----------------------------------------
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

// ---------------------------------------- 甯冨眬 ----------------------------------------
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

void AgObj_GetLocalBound(AgObj* obj, AgRect* bound) {
    bound->x = 0;
    bound->y = 0;
    bound->w = obj->bound.w;
    bound->h = obj->bound.h;
}

// ---------------------------------------- z鎿嶄綔 ----------------------------------------
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

// ---------------------------------------- 濂囨€殑鎿嶄綔 ----------------------------------------
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
        obj->vfunc.event(obj, event);
        obj = obj->parent;
    }
}