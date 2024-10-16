#include "ag_comp/ag_list_view.h"
#include "ag_impl/mem_impl.h"

// ---------------------------------------- impl ----------------------------------------
static void _Layout(AgObj* obj) {
    AgListView* lv = AGUI_CONTAINER_OF(AgListView, obj, obj);
    AgListView_SetBeginIdx(lv, lv->begin_idx);
}

static void _Obj_Draw(AgObj* obj, AgPainter* painter) {
    AgListView* lv = AGUI_CONTAINER_OF(AgListView, obj, obj->parent);
    ag_uint32 idx = lv->begin_idx + obj->id;
    ag_bool selected = lv->select_obj == obj;
    lv->model->draw(obj, painter, idx, selected);
}

// ---------------------------------------- private ----------------------------------------
static AgObj* GetOneObj(AgListView* lv) {
    AgListNode* node = AgList_Popfront(&lv->frees);
    if (NULL == node) {
        AgObj* obj = AgImpl_MemAlloc(sizeof(AgObj));
        AgObj_Init(obj);
        obj->vfunc.draw = _Obj_Draw;
        return obj;
    }
    else {
        return AGUI_CONTAINER_OF(AgObj, node, node);
    }
}

static void FreeAllDsiplayObj(AgListView* lv) {
    AgList list;
    AgList_Cut(&lv->obj.childern, &list, lv->obj.childern.head);
    AgList_Append(&lv->frees, &list);
}

/**
 * @brief 从idx处(包括)开始向上或者向下计算能够容纳多少物体
 * @param lv 不能为NULL,model不能为NULL
 * @param idx 开始索引
 * @param is_down ag_true向下计算,ag_false向上计算
 * @return 
 */
static ag_int32 GetHowManyItems(AgListView* lv, ag_int32 idx, ag_bool is_down) {
    ag_int16 y = 0;
    ag_int16 h = lv->obj.bound.h;
    ag_int32 count = 0;
    ag_int32 num_items = lv->model->count();
    
    if (ag_true == is_down) {
        for (;;) {
            if (idx >= num_items) {
                return count;
            }
            y += lv->model->height(idx++);
            if (y > h) {
                return count;
            }
            ++count;
        }
    }
    else {
        for (;;) {
            if (idx < 0) {
                return count;
            }
            y += lv->model->height(idx--);
            if (y > h) {
                return count;
            }
            ++count;
        }
    }
}

static void SetEndIdx(AgListView* lv, ag_int32 idx) {
    if (NULL == lv->model) {
        return;
    }
    idx = AGUI_MIN(lv->model->count() - 1, idx);
    idx = AGUI_MAX(0, idx);

    /* 计算起始索引和结束索引 */
    uint32_t items = GetHowManyItems(lv, idx, ag_false);
    lv->begin_idx = idx - items + 1;
    lv->display_count = items;

    /* 布局 */
    FreeAllDsiplayObj(lv);
    ag_int16 w = lv->obj.bound.w;
    ag_int16 y = 0;
    for (uint32_t i = 0; i < items; ++i) {
        AgObj* obj = GetOneObj(lv);
        obj->id = i; /* id用作idx使用,实际idx = lv.begin_idx + obj.id */

        ag_int16 h = lv->model->height(lv->begin_idx + i);
        AgObj_SetBounds(obj, 0, y, w, h);
        y += h;
        AgObj_AddChild(&lv->obj, obj);
    }
    lv->select_obj = AgObj_LastChild(&lv->obj);

    /* redraw */
    AgObj_Redraw(&lv->obj);
}

static void ScrollUp(AgListView* lv, ag_int32 shift) {
    if (shift > lv->select_obj->id) { /* 离开了当前范围 */
        /* 直接重定向 */
        AgListView_SetBeginIdx(lv, lv->begin_idx - shift);
    }
    else {
        AgObj_Redraw(lv->select_obj);
        /* 移动select到shifted */
        for (ag_int32 i = 0; i < shift; ++i) {
            lv->select_obj = AgObj_PrevSibling(lv->select_obj);
        }
        AgObj_Redraw(lv->select_obj);
    }
}

static void ScrollDown(AgListView* lv, ag_int32 shift) {
    ag_int32 shifed = lv->select_obj->id + shift;
    if (shifed >= lv->display_count) {
        /* 将最后一个重定向到lv.begin_idx+shift */
        SetEndIdx(lv, shifed + lv->begin_idx);
    }
    else {
        AgObj_Redraw(lv->select_obj);
        /* 移动select到shifted */
        for (ag_int32 i = 0; i < shift; ++i) {
            lv->select_obj = AgObj_NextSibling(lv->select_obj);
        }
        AgObj_Redraw(lv->select_obj);
    }
}

static void NullModelUpdate(AgListView* lv) {
    FreeAllDsiplayObj(lv);
    lv->begin_idx = 0;
    lv->display_count = 0;
    lv->select_obj = NULL;
    AgObj_Redraw(&lv->obj);
}

// ---------------------------------------- public ----------------------------------------

void AgListView_Init(AgListView* lv, AgObj* parent, struct __AgListModel* model) {
    AgObj_Init2(&lv->obj, parent);
    lv->model = model;
    AgList_Init(&lv->frees);
    lv->begin_idx = 0;
    lv->display_count = 0;
    lv->select_obj = NULL;
    
    lv->obj.vfunc.layout = _Layout;
    AgListView_SetModel(lv, model);
}

void AgListView_Destroy(AgListView* lv) {
    FreeAllDsiplayObj(lv);
    AgListNode* node = lv->frees.head;
    while (NULL != node) {
        AgListNode* next = node->next;
        AgImpl_MemFree(node);
        node = next;
    }
}

void AgListView_SetModel(AgListView* lv, struct __AgListModel* model) {
    lv->model = model;
    if (NULL == model) {
        NullModelUpdate(lv);
    }
    else {
        AgListView_SetBeginIdx(lv, 0);
    }
}

void AgListView_ItemChange(AgListView* lv, ag_int32 idx) {
    ag_int32 obj_idx = idx - lv->begin_idx;
    if (obj_idx < 0 || obj_idx >= lv->display_count) {
        return;
    }

    /* 重新绘制对应的obj */
    AgObj* obj = Agobj_FirstChild(&lv->obj);
    for (uint32_t i = 0; i < obj_idx; ++i) {
        obj = AgObj_NextSibling(obj);
    }
    AgObj_Redraw(obj);
}

void AgListView_Update(AgListView* lv) {
    if (NULL == lv->model) {
        NullModelUpdate(lv);
        return;
    }

    ag_int32 num_items = lv->model->count();
    if (num_items == 0) {
        NullModelUpdate(lv);
        return;
    }

    ag_int32 can_display = num_items - lv->begin_idx;
    if (can_display < lv->display_count) { /* 缩小了 */
        SetEndIdx(lv, lv->begin_idx + can_display);
    }
    else if (can_display > lv->display_count) { /* 扩大了 */
        AgListView_SetBeginIdx(lv, lv->begin_idx);
    }

    /* 更新obj */
    AgObj_Redraw(&lv->obj);
}

void AgListView_SetBeginIdx(AgListView* lv, ag_int32 idx) {
    if (NULL == lv->model) {
        return;
    }

    /* clamp idx */
    idx = AGUI_MIN(lv->model->count() - 1, idx);
    idx = AGUI_MAX(0, idx);
    lv->begin_idx = idx;

    /* how many items */
    ag_int32 items = GetHowManyItems(lv, idx, ag_true);
    lv->display_count = items;

    /* layout */
    FreeAllDsiplayObj(lv);
    ag_int16 w = lv->obj.bound.w;
    ag_int16 y = 0;
    for (ag_int32 i = 0; i < items; ++i) {
        AgObj* obj = GetOneObj(lv);
        obj->id = i; /* id用作idx使用,实际idx = lv.begin_idx + obj.id */
        
        ag_int16 h = lv->model->height(lv->begin_idx + i);
        AgObj_SetBounds(obj, 0, y, w, h);
        y += h;
        AgObj_AddChild(&lv->obj, obj);
    }
    lv->select_obj = Agobj_FirstChild(&lv->obj);

    /* redraw */
    AgObj_Redraw(&lv->obj);
}

void AgListView_Scroll(AgListView* lv, ag_int32 shift) {
    if (0 == shift) {
        return;
    }
    if (NULL == lv->model || NULL == lv->select_obj) {
        return;
    }
    if (shift < 0) {
        ScrollUp(lv, -shift);
    }
    else {
        ScrollDown(lv, +shift);
    }
}