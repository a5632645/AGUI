#include "agui/ag_obj.hpp"
#include "ag_impl/ag_cfg.hpp"
#include "ag_impl/ag_impl.hpp"
#include "agui/ag_color.hpp"

namespace agui {
// ---------------------------------------- private ----------------------------------------
void AgObj::_PopUpRedraw(const AgRect& bound) {
    if (ag_true == flags_.redraw) {
        return;
    }
    if (ag_false == flags_.visiable) {
        return;
    }
    
    AgRect rect = bound;
    rect.x += bound.x;
    rect.y += bound.y;
    flags_.invalid = ag_true;
    
    /* 同层级如果重叠也要绘制 */
    auto next_it = NextSibling();
    while (next_it) {
        auto* other = next_it.ptr;
        if (ag_false == other->flags_.redraw && ag_true == other->flags_.visiable) {
            if (ag_true == rect.IsIntersected(other->bound_)) {
                if (ag_false == other->flags_.transpant) {
                    if (other->parent_ != nullptr) {
                        other->parent_->_PopUpRedraw(other->bound_);
                    }
                    other->flags_.redraw = ag_true;
                    other->flags_.invalid = ag_true;
                }
                else if (nullptr != other->parent_) {
                    other->parent_->_ReDraw();
                }
            }
        }
        next_it = next_it->NextSibling();
    }
    if (parent_ != nullptr) {
        parent_->_PopUpRedraw(rect);
    }
}

void AgObj::_ReDraw() {
    _PopUpRedraw(GetLocalBound());
    flags_.redraw = ag_true;
    flags_.invalid = ag_true;
}

void AgObj::DrawObjInObj(AgPainter& painter) {
    /* 保存绘制区 */
    painter.SaveState();
    /* 移动绘制区 */
    painter.draw_aera_.x += bound_.x;
    painter.draw_aera_.y += bound_.y;
    painter.draw_aera_.w = bound_.w;
    painter.draw_aera_.h = bound_.h;

    if (ag_true == flags_.redraw && ag_true == flags_.visiable) {
        painter.always_redraw_ = ag_true;
    }
    
    if (ag_true == painter.always_redraw_) {
        /* 如果always_redraw置位，忽视标记 */
        flags_.invalid = ag_false;
        if (ag_true == flags_.visiable) {
            flags_.redraw = ag_false;
            Draw(painter);
            /* 绘制子节点 */
            childern_.ForEach([&painter](AgListNode* child){
                child->As<AgObj>()->DrawObjInObj(painter);
            });
        }
    }
    else {
        if (ag_true == flags_.invalid) {
            flags_.invalid = ag_false;
            if (ag_true == flags_.visiable) {
                /* 绘制子节点 */
                childern_.ForEach([&painter](AgListNode* child){
                    child->As<AgObj>()->DrawObjInObj(painter);
                });
            }
        }
    }

    /* 还原绘制区 */
    painter.RestoreState();
}

// ---------------------------------------- public ----------------------------------------
void AgObj::Draw(AgPainter& painter) {
    impl::AgEmptyObjDraw(*this, painter);
}

AgObj::~AgObj() {
    RemoveAllChild();
}

NullablePtr<AgObj> AgObj::FirstChild() {
    return childern_.GetHead()->As<AgObj>();
}

NullablePtr<AgObj> AgObj::NextSibling() {
    if (nullptr == AgObj::AgListNode::next_) {
        return nullptr;
    }
    return AgObj::AgListNode::next_->As<AgObj>();
}

NullablePtr<AgObj> AgObj::PrevSibling() {
    if (nullptr == AgObj::AgListNode::prev_) {
        return nullptr;
    }
    return AgObj::AgListNode::prev_->As<AgObj>();
}

NullablePtr<AgObj> AgObj::LastChild() {
    if (nullptr == childern_.GetTail()) {
        return nullptr;
    }
    return childern_.GetTail()->As<AgObj>();
}

AgObj::AgObj(AgObj* parent) {
    flags_.redraw = ag_true;
    flags_.invalid = ag_true;
    flags_.visiable = ag_true;
    flags_.transpant = ag_false;
    parent_ = parent;
    if (nullptr != parent_) {
        parent_->AddChild(*this);
    }
}

void AgObj::AddChild(AgObj& child) {
    childern_.PushBack(child);
    child.parent_ = this;
}

void AgObj::AddChildAtBack(AgObj& child) {
    childern_.PushFront(child);
    child.parent_ = this;
}

void AgObj::RemoveChild(AgObj& child) {
    if (child.parent_ != this) {
        return;
    }
    childern_.Remove(child);
    child.parent_ = nullptr;
}

void AgObj::DrawObj(AgPainter& painter) {
    painter.PrepareDraw(GetLocalBound());
    painter.BeginFrame();
    DrawObjInObj(painter);
    painter.EndFrame();
}

void AgObj::AddChildFromArray(AgObj* childs, ag_uint32 count) {
    for (ag_uint32 i = 0; i < count; ++i) {
        childern_.PushBack(childs[i]);
        childs[i].parent_ = this;
    }
}

void AgObj::RemoveAllChild() {
    childern_.RemoveAll([](AgListNode* node) {
        node->As<AgObj>()->parent_ = nullptr;
    });
}

void AgObj::LeaveParent() {
    if (nullptr != parent_) {
        parent_->RemoveChild(*this);
    }
}

ag_bool AgObj::HitTest(ag_int16 x, ag_int16 y) {
    if (x < bound_.x || x >= bound_.x + bound_.w) {
        return ag_false;
    }
    if (y < bound_.y || y >= bound_.y + bound_.h) {
        return ag_false;
    }
    return ag_true;
}

NullablePtr<AgObj> AgObj::HitObj(ag_int16 x, ag_int16 y) {
    if (ag_true == HitTest(x, y) && ag_true == flags_.visiable) {
        if (ag_true == childern_.IsEmpty()) {
            return this;
        }
        else {
            x -= bound_.x;
            y -= bound_.y;
            for (auto& node : childern_) {
                if (ag_true == node.As<AgObj>()->HitTest(x, y)) {
                    return node.As<AgObj>();
                }
            }
            return this;
        }
    }
    return nullptr;
}

void AgObj::SetVisible(ag_bool visiable) {
    if (flags_.visiable == visiable) {
        return;
    }
    flags_.visiable = visiable;
    if (nullptr != parent_) {
        parent_->_ReDraw();
    }
}

void AgObj::ReDraw() {
    if (ag_false == flags_.transpant) {
        _ReDraw();
    }
    else {
        if (nullptr != parent_) {
            parent_->_ReDraw();
        }
    }
}

void AgObj::RedrawRoot() {
    AgObj* obj = this;
    while (nullptr != obj->parent_) {
        obj = obj->parent_;
    }
    obj->flags_.redraw = ag_true;
    obj->flags_.invalid = ag_true;
}

void AgObj::DoLayout() {
    Layout();
}

ag_bool AgObj::NeedDraw() const {
    return flags_.invalid;
}

void AgObj::SetBound(const AgRect& bound) {
    if (bound_ == bound) {
        return;
    }
    bound_ = bound;
    Layout();
    if (nullptr != parent_) {
        parent_->ReDraw();
    }
}

void AgObj::SetBounds(ag_int16 x, ag_int16 y, ag_int16 w, ag_int16 h) {
    AgRect rect = {x, y, w, h};
    AgObj::SetBound(rect);
}

void AgObj::SetPos(ag_int16 x, ag_int16 y) {
    if (x == bound_.x && y == bound_.y) {
        return;
    }
    AgRect rect = {x, y, bound_.w, bound_.h};
    AgObj::SetBound(rect);
}

void AgObj::SetSize(ag_int16 w, ag_int16 h) {
    if (w == bound_.w && h == bound_.h) {
        return;
    }
    AgRect rect = {bound_.x, bound_.y, w, h};
    AgObj::SetBound(rect);
}

AgRect AgObj::GetLocalBound() const {
    return AgRect{
        0, 0, bound_.w, bound_.h
    };
}

void AgObj::BringToFront() {
    if (NULL == parent_) {
        return;
    }
    parent_->childern_.Remove(*this);
    parent_->childern_.PushBack(*this);
    parent_->ReDraw();
}

void AgObj::SendToBack() {
    if (nullptr == parent_) {
        return;
    }
    parent_->childern_.Remove(*this);
    parent_->childern_.PushFront(*this);
    parent_->ReDraw();
}

void AgObj::_CalcSingleBound() {
    if (ag_false == childern_.IsEmpty()
        && (bound_.w == 0 || bound_.h == 0)) {
        AgRect rect = {
            .x = 32767,
            .y = 32767,
            .w = -32768,
            .h = -32768
        };

        for (auto& node : childern_) {
            auto& child = *node.As<AgObj>();
            rect.x = AGUI_MIN(rect.x, child.bound_.x);
            rect.y = AGUI_MIN(rect.y, child.bound_.y);
            rect.w = AGUI_MAX(rect.w, child.bound_.x + child.bound_.w);
            rect.h = AGUI_MAX(rect.h, child.bound_.y + child.bound_.h);
        }

        rect.w -= rect.x;
        rect.h -= rect.y;
        rect.x += bound_.x;
        rect.y += bound_.y;

        bound_ = rect;
    }
}

void AgObj::CalcBound() {
    childern_.ForEach([](AgListNode* node) {
        node->As<AgObj>()->_CalcSingleBound();
    });
    _CalcSingleBound();
}

void AgObj::SendEvent(AgEvent& event) {
    AgObj* obj = this;
    while (nullptr != obj && ag_false == event.handled) {
        AgObj* parent = parent_;
        parent->Event(event);
        obj = parent;
    }
}

void AgObj::FastLayout(AgAlignEnum align) {
    if (nullptr == parent_) {
        return;
    }

    using enum AgAlignEnum;

    AgAlignEnum x_align = AGUI_X_ALIGN(align);
    const AgRect& parent_bound = parent_->bound_;
    const AgRect& bound = bound_;
    switch (x_align) {
    case kXCenter:
        SetPos(parent_bound.w / 2 - bound.w / 2, bound.y);
        break;
    case kXLeft:
        SetPos(0, bound.y);
        break;
    case kXRight:
        SetPos(parent_bound.w - bound.w, bound.y);
        break;
    default:
        break;
    }

    AgAlignEnum y_align = AGUI_Y_ALIGN(align);
    switch (y_align) {
    case kYCenter:
        SetPos(bound.x, parent_bound.h / 2 - bound.h / 2);
        break;
    case kYTop:
        SetPos(bound.x, 0);
        break;
    case kYBottom:
        SetPos(bound.x, parent_bound.h - bound.h);
        break;
    default:
        break;
    }
}

}
