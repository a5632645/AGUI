#include "ag_comp/ag_list_view.hpp"
#include "ag_impl/default_impl.hpp"

namespace agui {

// ---------------------------------------- delegate ----------------------------------------
void AgListView::EmptyDelegate::Background(AgPainter& painter, const AgRect& draw_aera) {
    AgObj obj;
    obj.SetBound(draw_aera);
    __EmptyObjDraw(obj, painter);
}

// ---------------------------------------- impl ----------------------------------------
void AgListView::__Draw_Vertical(AgPainter& painter) {
    ag_int16 final_y = 0;
    ag_int16 item_w = bound_.w;
    
    if (NULL != model_) {
        ag_int16 num_items = model_->Count();
        if (display_count_ < num_items) {
            ag_int16 w = delegate_->ScrollBarWidth(style_);
            item_w -= w;
            
            ag_int16 x = bound_.w - w;
            ag_int16 y = bound_.h * begin_idx_ / num_items;
            ag_int16 y_end = bound_.h * (begin_idx_ + display_count_) / num_items;
            
            painter.SaveState();
            painter.ModifyCurrDrawAera().x = x;
            painter.ModifyCurrDrawAera().w = w;
            delegate_->ScrollBar(painter, y, y_end);
            painter.RestoreState();
        }

        painter.SaveState();
        painter.ModifyCurrDrawAera().w = item_w;
        for (ag_int16 i = 0; i < display_count_; ++i) {
            ag_int16 idx = begin_idx_ + i;
            ag_int16 h = model_->Length(style_, idx);
            painter.ModifyCurrDrawAera().h = h;
            model_->Draw(painter, idx, select_idx_ == idx);
            painter.ModifyCurrDrawAera().y += h;
        }
        final_y = painter.ModifyCurrDrawAera().y;
        painter.RestoreState();
    }

    /* 填充背景 */
    AgRect fill = {
        .x = 0,
        .y = final_y,
        .w = item_w,
        .h = static_cast<ag_int16>(bound_.h - final_y)
    };
    delegate_->Background(painter, fill);
}

void AgListView::__Draw_Horizontal(AgPainter& painter) {
    ag_int16 final_x = 0;
    ag_int16 item_h = bound_.h;
    
    if (NULL != model_) {
        ag_int16 num_items = model_->Count();
        if (display_count_ < num_items) {
            ag_int16 h = delegate_->ScrollBarWidth(style_);
            item_h -= h;
            
            ag_int16 y = bound_.h - h;
            ag_int16 x = bound_.w * begin_idx_ / num_items;
            ag_int16 x_end = bound_.w * (begin_idx_ + display_count_) / num_items;
            
            painter.SaveState();
            painter.ModifyCurrDrawAera().y = y;
            painter.ModifyCurrDrawAera().h = h;
            delegate_->ScrollBar(painter, x, x_end);
            painter.RestoreState();
        }

        painter.SaveState();
        painter.ModifyCurrDrawAera().h = item_h;
        for (ag_int16 i = 0; i < display_count_; ++i) {
            ag_int16 idx = begin_idx_ + i;
            ag_int16 w = model_->Length(style_, idx);
            painter.ModifyCurrDrawAera().w = w;
            model_->Draw(painter, idx, select_idx_ == idx);
            painter.ModifyCurrDrawAera().x += w;
        }
        final_x = painter.ModifyCurrDrawAera().x;
        painter.RestoreState();
    }

    /* 填充背景 */
    AgRect fill = {
        .x = final_x,
        .y = 0,
        .w = static_cast<ag_int16>(bound_.w - final_x),
        .h = item_h,
    };
    delegate_->Background(painter, fill);
}

void AgListView::Layout() {
    SetBeginIdx(begin_idx_);
}

// ---------------------------------------- private ----------------------------------------

/**
 * @brief 从idx处(包括)开始向上或者向下计算能够容纳多少物体
 * @param lv 不能为NULL,model不能为NULL
 * @param idx 开始索引
 * @param is_down ag_true向下计算,ag_false向上计算
 * @return 
 */
ag_int16 AgListView::_GetHowManyItems(ag_int16 idx, ag_bool is_down) {
    ag_int16 h = bound_.h;
    if (style_ == AgListViewStyle::eAgListViewStyle_Horizontal) {
        h = bound_.w;
    }

    ag_int16 y = 0;
    ag_int32 count = 0;
    ag_int32 num_items = model_->Count();
    
    if (ag_true == is_down) {
        for (;;) {
            if (idx >= num_items) {
                return count;
            }
            y += model_->Length(style_, idx++);
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
            y += model_->Length(style_, idx--);
            if (y > h) {
                return count;
            }
            ++count;
        }
    }
}

void AgListView::_SetEndIdx(ag_int16 idx) {
    idx = AGUI_MIN(idx, model_->Count() - 1);
    idx = AGUI_MAX(idx, 0);
    /* 在调用此处时一定是有效的model */
    display_count_ = _GetHowManyItems(idx, ag_false);
    begin_idx_ = idx - display_count_ + 1;
    if (select_idx_ > idx) { /* 向下溢出当前范围 */
        select_idx_ = idx;
    }
    ReDraw();
}

void AgListView::_ScrollUp(ag_int16 shift) {
    ag_int16 new_select = select_idx_ - shift;
    new_select = AGUI_MIN(new_select, model_->Count() - 1);
    new_select = AGUI_MAX(new_select, 0);
    ag_int16 win_begin = begin_idx_;
    if (new_select < win_begin) {
        /* 向上溢出当前范围，直接设定顶部到对应位置 */
        SetBeginIdx(new_select);
        select_idx_ = begin_idx_;
    }
    else {
        /* 滚动过去 */
        select_idx_ = new_select;
        ReDraw();
    }
}

void AgListView::_ScrollDown(ag_int16 shift) {
    ag_int16 new_select = select_idx_ + shift;
    ag_int16 win_end = begin_idx_ + display_count_;
    if (new_select >= win_end) {
        /* 向下溢出当前范围，直接设定底部到对应位置 */
        _SetEndIdx(new_select);
        select_idx_ = begin_idx_ + display_count_ - 1;
        select_idx_ = AGUI_MAX(select_idx_, 0);
    }
    else {
        /* 滚动过去 */
        select_idx_ = new_select;
        ReDraw();
    }
}

void AgListView::_NullModelUpdate() {
    begin_idx_ = 0;
    display_count_ = 0;
    select_idx_ = 0;
    ReDraw();
}

void AgListView::Draw(AgPainter& painter) {
    if (style_ == AgListViewStyle::eAgListViewStyle_Vertical) {
        __Draw_Horizontal(painter);
    }
    else {
        __Draw_Vertical(painter);
    }
}

void AgListView::Event(AgEvent& event) {
    if (NULL == model_) {
        return;
    }
    model_->Event(*this, event, select_idx_);
}

// ---------------------------------------- public ----------------------------------------

AgListView::AgListView(AgObj* parent)
    : AgObj(parent, eAgObjType_ListView) {
    SetModel(nullptr);
}

void AgListView::SetModel(AgListModel* model) {
    model = model;
    Update();
}

void AgListView::SetDelegate(AgListViewDelegate* delegate) {
    delegate_ = delegate;
    if (NULL == delegate) {
        delegate_ = &s_empty_delegate_;
    }
    ReDraw();
}

void AgListView::NotifyItemChange(ag_int16 idx) {
    if (NULL == model_) {
        _NullModelUpdate();
        return;
    }
    ag_int16 win_begin = begin_idx_;
    ag_int16 win_end = begin_idx_ + display_count_;
    if (idx < win_begin || idx >= win_end) {
        return;
    }
    ReDraw();
}

void AgListView::Update() {
    if (NULL == model_) {
        _NullModelUpdate();
        return;
    }

    ag_int16 num_items = model_->Count();
    if (num_items <= begin_idx_ + display_count_) { /* 比当前更小 */
        ag_int16 new_end = num_items - 1;
        new_end = AGUI_MAX(new_end, 0);
        _SetEndIdx(new_end);
    }
    else if (num_items > begin_idx_ + display_count_) { /* 比当前更大 */
        SetBeginIdx(begin_idx_);
    }

    ReDraw();
}

void AgListView::SetBeginIdx(ag_int16 idx) {
    if (NULL == model_) {
        _NullModelUpdate();
        return;
    }

    ag_int16 old_begin = begin_idx_;
    idx = AGUI_MIN(idx, model_->Count() - 1);
    idx = AGUI_MAX(idx, 0);
    display_count_ = _GetHowManyItems(idx, ag_true);
    begin_idx_ = idx;
    
    ag_int16 win_begin = begin_idx_;
    ag_int16 win_end = begin_idx_ + display_count_;
    if (select_idx_ < win_begin || select_idx_ >= win_end) {
        if (begin_idx_ > old_begin) {
            select_idx_ = begin_idx_;
        }
        else {
            select_idx_ = begin_idx_ + display_count_ - 1;
            select_idx_ = AGUI_MAX(select_idx_, 0);
        }
    }

    ReDraw();
}

void AgListView::Scroll(ag_int16 shift) {
    if (NULL == model_) {
        return;
    }
    if (shift == 0) {
        return;
    }
    if (shift < 0) {
        _ScrollUp(-shift);
    }
    else {
        _ScrollDown(shift);
    }
}

void AgListView::SetSelectedIdx(ag_int16 idx) {
    if (NULL == model_) {
        return;
    }
    ag_int16 win_begin = begin_idx_;
    ag_int16 win_end = begin_idx_ + display_count_;
    if (idx < win_begin || idx >= win_end) {
        return;
    }
    select_idx_ = idx;
    ReDraw();
}

void AgListView::SetStyle(AgListViewStyle style) {
    if (style_ == style) {
        return;
    }
    style = style;
    ReDraw();
}

}
