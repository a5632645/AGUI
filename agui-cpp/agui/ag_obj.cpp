#include "agui/ag_obj.hpp"
#include "ag_impl/ag_cfg.hpp"
#include "ag_impl/default_impl.hpp"

namespace agui {

// ---------------------------------------- debug ----------------------------------------
#if AGUI_DBG_WIRE_FRAME == 1
static constexpr AgColor Hue2Rgb(uint8_t hue, uint8_t brightness) {
    uint8_t r = 0, g = 0, b = 0;
    if(hue < 60) {
        r = brightness;
        g = hue * 3 * brightness / 100;
        b = 0;
    } else if(hue < 120) {
        r = brightness - hue * 3 * brightness / 100;
        g = brightness;
        b = 0;
    } else if(hue < 180) {
        r = 0;
        g = brightness;
        b = hue * 3 * brightness / 100;
    } else if(hue < 240) {
        r = 0;
        g = brightness - hue * 3 * brightness / 100;
        b = brightness;
    } else if(hue < 300) {
        r = hue * 3 * brightness / 100;
        g = 0;
        b = brightness;
    } else {
        r = brightness;
        g = 0;
        b = brightness - hue * 3 * brightness / 100;
    }
    return (AgColor){r, g, b, 255};
}

static AgColor AgDbg_RandomColor() {
    static uint8_t last_hue = 0;
    last_hue += 60;
    last_hue %= 360;
    return Hue2Rgb(last_hue, 255);
}

void AgObj::_DrawDbgFrame(AgPainter& painter) {
    AgRect rect = GetLocalBound();
    AgColor color = AgDbg_RandomColor();
    __DrawColorFrame(painter, rect, color);
}
#else
void AgObj::_DrawDbgFrame(AgPainter& painter) {}
#endif

// ---------------------------------------- private ----------------------------------------
#if AGUI_NO_OBJ_OVERLAY == 0
void AgObj::_PopUpRedraw(const AgRect* bound) {
    if (NULL == obj) {
        return;
    }
    if (ag_true == flags_.redraw) {
        return;
    }
    if (ag_false == flags_.visiable) {
        return;
    }
    
    AgRect rect = *bound;
    rect.x += bound.x;
    rect.y += bound.y;
    flags_.invalid = ag_true;
    
    /* 同层级如果重叠也要绘制 */
    AgListNode* node = node.next;
    while (NULL != node) {
        AgObj* other = AGUI_CONTAINER_OF(AgObj, node, node);
        if (ag_false == other->flags_.redraw && ag_true == other->flags_.visiable) {
            if (ag_true == AgRect_IsIntersected(&rect, &other->bound)) {
                if (ag_false == other->flags_.transpant) {
                    __PopUpRedraw(other->parent, &other->bound);
                    other->flags_.redraw = ag_true;
                    other->flags_.invalid = ag_true;
                }
                else {
                    _ReDraw(other->parent);
                }
            }
        }
        node = node->next;
    }
    __PopUpRedraw(parent, &rect);
}

static void _ReDraw() {
    if (NULL == obj) {
        return;
    }
    AgRect b;
    AgObj::GetLocalBound(obj, &b);
    __PopUpRedraw(obj, &b);
    flags_.redraw = ag_true;
    flags_.invalid = ag_true;
}
#else
void AgObj::_PopUpRedraw() {}
void AgObj::_ReDraw() {
    if (ag_false == flags_.visiable) {
        return;
    }
    
    flags_.redraw = ag_true;
    flags_.invalid = ag_true;

    AgObj* obj = this;
    while (NULL != obj) {
        if (ag_true == flags_.transpant) {
            if (nullptr != parent_) {
                parent_->_ReDraw();
            }
        }
        else {
            flags_.invalid = ag_true;
            obj = parent_;
        }
    }
}
#endif

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
            /* 调试绘制边框 */
            _DrawDbgFrame(painter);
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
    __EmptyObjDraw(*this, painter);
}

AgObj::~AgObj() {
    RemoveAllChild();
}

NullablePtr<AgObj> AgObj::FirstChild() {
    return childern_.GetHead()->As<AgObj>();
}

NullablePtr<AgObj> AgObj::NextSibling() {
    if (nullptr == AgListNode::next_) {
        return nullptr;
    }
    return AgListNode::next_->As<AgObj>();
}

NullablePtr<AgObj> AgObj::PrevSibling() {
    if (NULL == AgListNode::prev_) {
        return NULL;
    }
    return AgListNode::prev_->As<AgObj>();
}

NullablePtr<AgObj> AgObj::LastChild() {
    if (NULL == childern_.GetTail()) {
        return NULL;
    }
    return childern_.GetTail()->As<AgObj>();
}

AgObj::AgObj(AgObj* parent, ag_uint16 type) : obj_type_(type) {
    flags_.redraw = ag_true;
    flags_.invalid = ag_true;
    flags_.visiable = ag_true;
    flags_.transpant = ag_false;
    parent_ = parent;
    if (NULL != parent_) {
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
    child.parent_ = NULL;
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
        node->As<AgObj>()->parent_ = NULL;
    });
}

void AgObj::LeaveParent() {
    if (NULL != parent_) {
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
    return NULL;
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
    while (NULL != obj->parent_) {
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
    if (NULL == parent_) {
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
    while (NULL != obj && ag_false == event.handled) {
        AgObj* parent = parent_;
        parent->Event(event);
        obj = parent;
    }
}

void AgObj::FastLayout(AgAlignEnum align) {
    if (NULL == parent_) {
        return;
    }

    AgAlignEnum x_align = AGUI_X_ALIGN(align);
    const AgRect& parent_bound = parent_->bound_;
    const AgRect& bound = bound_;
    switch (x_align) {
    case eAgAlign_XCenter:
        SetPos(parent_bound.w / 2 - bound.w / 2, bound.y);
        break;
    case eAgAlign_XLeft:
        SetPos(0, bound.y);
        break;
    case eAgAlign_XRight:
        SetPos(parent_bound.w - bound.w, bound.y);
        break;
    default:
        break;
    }

    AgAlignEnum y_align = AGUI_Y_ALIGN(align);
    switch (y_align) {
    case eAgAlign_YCenter:
        SetPos(bound.x, parent_bound.h / 2 - bound.h / 2);
        break;
    case eAgAlign_YTop:
        SetPos(bound.x, 0);
        break;
    case eAgAlign_YBottom:
        SetPos(bound.x, parent_bound.h - bound.h);
        break;
    default:
        break;
    }
}

}
