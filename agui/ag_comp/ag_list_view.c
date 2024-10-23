#include "ag_comp/ag_list_view.h"

// ---------------------------------------- impl ----------------------------------------
static void __Draw_Vertical(AgObj* obj, AgPainter* painter) {
    AgListView* lv = AGUI_CONTAINER_OF(AgListView, obj, obj);
    ag_int16 final_y = 0;
    ag_int16 item_w = obj->bound.w;
    
    if (NULL != lv->model) {
        ag_int16 num_items = lv->model->count(lv->model);
        if (lv->display_count < num_items) {
            ag_int16 w = lv->scroll_bar_width;
            item_w -= w;
            
            ag_int16 x = obj->bound.w - w;
            ag_int16 y = obj->bound.h * lv->begin_idx / num_items;
            ag_int16 y_end = obj->bound.h * (lv->begin_idx + lv->display_count) / num_items;
            
            AgPainter_SaveState(painter);
            painter->draw_aera.x = x;
            painter->draw_aera.w = w;
            lv->scroll_bar(painter, y, y_end);
            AgPainter_RestoreState(painter);
        }

        AgPainter_SaveState(painter);
        painter->draw_aera.w = item_w;
        for (ag_int16 i = 0; i < lv->display_count; ++i) {
            ag_int16 idx = lv->begin_idx + i;
            ag_int16 h = lv->model->height(lv, idx);
            painter->draw_aera.h = h;
            lv->model->draw(painter, idx, lv->select_idx == idx);
            painter->draw_aera.y += h;
        }
        final_y = painter->draw_aera.y;
        AgPainter_RestoreState(painter);
    }

    /* 填充背景 */
    AgRect fill = {
        .x = 0,
        .y = final_y,
        .w = item_w,
        .h = obj->bound.h - final_y
    };
    lv->background(painter, &fill);
}

static void __Draw_Horizontal(AgObj* obj, AgPainter* painter) {
    AgListView* lv = AGUI_CONTAINER_OF(AgListView, obj, obj);
    ag_int16 final_x = 0;
    ag_int16 item_h = obj->bound.h;
    
    if (NULL != lv->model) {
        ag_int16 num_items = lv->model->count(lv->model);
        if (lv->display_count < num_items) {
            ag_int16 h = lv->scroll_bar_width;
            item_h -= h;
            
            ag_int16 y = obj->bound.h - h;
            ag_int16 x = obj->bound.w * lv->begin_idx / num_items;
            ag_int16 x_end = obj->bound.w * (lv->begin_idx + lv->display_count) / num_items;
            
            AgPainter_SaveState(painter);
            painter->draw_aera.y = y;
            painter->draw_aera.h = h;
            lv->scroll_bar(painter, x, x_end);
            AgPainter_RestoreState(painter);
        }

        AgPainter_SaveState(painter);
        painter->draw_aera.h = item_h;
        for (ag_int16 i = 0; i < lv->display_count; ++i) {
            ag_int16 idx = lv->begin_idx + i;
            ag_int16 w = lv->model->height(lv, idx);
            painter->draw_aera.w = w;
            lv->model->draw(painter, idx, lv->select_idx == idx);
            painter->draw_aera.x += w;
        }
        final_x = painter->draw_aera.x;
        AgPainter_RestoreState(painter);
    }

    /* 填充背景 */
    AgRect fill = {
        .x = final_x,
        .y = 0,
        .w = obj->bound.w - final_x,
        .h = item_h,
    };
    lv->background(painter, &fill);
}

static void __Layout(AgObj* obj) {
    AgListView* lv = AGUI_CONTAINER_OF(AgListView, obj, obj);
    AgListView_SetBeginIdx(lv, lv->begin_idx);
}

// ---------------------------------------- private ----------------------------------------

/**
 * @brief 从idx处(包括)开始向上或者向下计算能够容纳多少物体
 * @param lv 不能为NULL,model不能为NULL
 * @param idx 开始索引
 * @param is_down ag_true向下计算,ag_false向上计算
 * @return 
 */
static ag_int16 _GetHowManyItems(AgListView* lv, ag_int16 idx, ag_bool is_down) {
    ag_int16 h = lv->obj.bound.h;
    if (lv->style == eAgListViewStyle_Horizontal) {
        h = lv->obj.bound.w;
    }

    ag_int16 y = 0;
    ag_int32 count = 0;
    ag_int32 num_items = lv->model->count(lv->model);
    
    if (ag_true == is_down) {
        for (;;) {
            if (idx >= num_items) {
                return count;
            }
            y += lv->model->height(lv, idx++);
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
            y += lv->model->height(lv, idx--);
            if (y > h) {
                return count;
            }
            ++count;
        }
    }
}

static void _SetEndIdx(AgListView* lv, ag_int16 idx) {
    idx = AGUI_MIN(idx, lv->model->count(lv->model) - 1);
    idx = AGUI_MAX(idx, 0);
    /* 在调用此处时一定是有效的model */
    lv->display_count = _GetHowManyItems(lv, idx, ag_false);
    lv->begin_idx = idx - lv->display_count + 1;
    if (lv->select_idx > idx) { /* 向下溢出当前范围 */
        lv->select_idx = idx;
    }
    AgObj_Redraw(&lv->obj);
}

static void _ScrollUp(AgListView* lv, ag_int16 shift) {
    ag_int16 new_select = lv->select_idx - shift;
    new_select = AGUI_MAX(new_select, 0);
    new_select = AGUI_MIN(new_select, lv->model->count(lv->model));
    ag_int16 win_begin = lv->begin_idx;
    if (new_select < win_begin) {
        /* 向上溢出当前范围，直接设定顶部到对应位置 */
        AgListView_SetBeginIdx(lv, new_select);
        lv->select_idx = lv->begin_idx;
    }
    else {
        /* 滚动过去 */
        lv->select_idx = new_select;
        AgObj_Redraw(&lv->obj);
    }
}

static void _ScrollDown(AgListView* lv, ag_int16 shift) {
    ag_int16 new_select = lv->select_idx + shift;
    ag_int16 win_end = lv->begin_idx + lv->display_count;
    if (new_select >= win_end) {
        /* 向下溢出当前范围，直接设定底部到对应位置 */
        _SetEndIdx(lv, new_select);
        lv->select_idx = lv->begin_idx + lv->display_count - 1;
        lv->select_idx = AGUI_MAX(lv->select_idx, 0);
    }
    else {
        /* 滚动过去 */
        lv->select_idx = new_select;
        AgObj_Redraw(&lv->obj);
    }
}

static void _NullModelUpdate(AgListView* lv) {
    lv->begin_idx = 0;
    lv->display_count = 0;
    lv->select_idx = 0;
    AgObj_Redraw(&lv->obj);
}

// ---------------------------------------- public ----------------------------------------

void AgListView_Init(AgListView* lv, AgObj* parent, struct __AgListModel* model) {
    AgObj_Init2(&lv->obj, parent);
    lv->style = eAgListViewStyle_Vertical;
    lv->obj.vfunc.draw = __Draw_Vertical;
    lv->obj.vfunc.layout = __Layout;
    lv->obj.obj_type = eAgObjType_ListView;
    AgListView_SetModel(lv, model);
}

void AgListView_SetModel(AgListView* lv, struct __AgListModel* model) {
    lv->model = model;
    AgListView_Update(lv);
}

void AgListView_ItemChange(AgListView* lv, ag_int16 idx) {
    if (NULL == lv->model) {
        _NullModelUpdate(lv);
        return;
    }
    ag_int16 win_begin = lv->begin_idx;
    ag_int16 win_end = lv->begin_idx + lv->display_count;
    if (idx < win_begin || idx >= win_end) {
        return;
    }
    AgObj_Redraw(&lv->obj);
}

void AgListView_Update(AgListView* lv) {
    if (NULL == lv->model) {
        _NullModelUpdate(lv);
        return;
    }

    if (lv->model->count(lv->model) <= lv->begin_idx + lv->display_count) { /* 比当前更小 */
        ag_int16 new_end = lv->model->count(lv->model) - 1;
        new_end = AGUI_MAX(new_end, 0);
        _SetEndIdx(lv, new_end);
    }
    else if (lv->model->count(lv->model) > lv->begin_idx + lv->display_count) { /* 比当前更大 */
        AgListView_SetBeginIdx(lv, lv->begin_idx);
    }

    AgObj_Redraw(&lv->obj);
}

void AgListView_SetBeginIdx(AgListView* lv, ag_int16 idx) {
    if (NULL == lv->model) {
        _NullModelUpdate(lv);
        return;
    }

    ag_int16 old_begin = lv->begin_idx;
    idx = AGUI_MIN(idx, lv->model->count(lv->model) - 1);
    idx = AGUI_MAX(idx, 0);
    lv->display_count = _GetHowManyItems(lv, idx, ag_true);
    lv->begin_idx = idx;
    
    ag_int16 win_begin = lv->begin_idx;
    ag_int16 win_end = lv->begin_idx + lv->display_count;
    if (lv->select_idx < win_begin || lv->select_idx >= win_end) {
        if (lv->begin_idx > old_begin) {
            lv->select_idx = lv->begin_idx;
        }
        else {
            lv->select_idx = lv->begin_idx + lv->display_count - 1;
            lv->select_idx = AGUI_MAX(lv->select_idx, 0);
        }
    }

    AgObj_Redraw(&lv->obj);
}

void AgListView_Scroll(AgListView* lv, ag_int16 shift) {
    if (NULL == lv->model) {
        return;
    }
    if (shift == 0) {
        return;
    }
    if (shift < 0) {
        _ScrollUp(lv, -shift);
    }
    else {
        _ScrollDown(lv, shift);
    }
}

void AgListView_SetSelectIdx(AgListView* lv, ag_int16 idx) {
    if (NULL == lv->model) {
        return;
    }
    ag_int16 win_begin = lv->begin_idx;
    ag_int16 win_end = lv->begin_idx + lv->display_count;
    if (idx < win_begin || idx >= win_end) {
        return;
    }
    lv->select_idx = idx;
    AgObj_Redraw(&lv->obj);
}

void AgListView_SetStyle(AgListView* lv, AgListViewStyle style) {
    if (style == lv->style) {
        return;
    }
    lv->style = style;

    if (lv->style == eAgListViewStyle_Horizontal) {
        lv->obj.vfunc.draw = __Draw_Horizontal;
    }
    else {
        lv->obj.vfunc.draw = __Draw_Vertical;
    }

    AgObj_Redraw(&lv->obj);
}
