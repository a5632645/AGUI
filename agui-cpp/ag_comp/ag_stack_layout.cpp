#include "ag_comp/ag_stack_layout.hpp"

namespace agui {

// ---------------------------------------- private ----------------------------------------
NullablePtr<AgObj> AgStackLayout::_ChangeObj(AgObj& obj) {
    auto curr = Current();
    if (curr) {
        curr->LeaveParent();
    }
    AddChild(obj);
    Layout();
    ReDraw();
    return curr;
}

NullablePtr<AgObj> AgStackLayout::_ChangeObj(NullablePtr<AgObj> obj) {
    if (!obj) {
        if (childern_.IsEmpty()) {
            return nullptr;
        }
        else {
            auto ret = FirstChild();
            ret->LeaveParent();
            return ret;
        }
    }
    return _ChangeObj(*obj);
}

// ---------------------------------------- impl ----------------------------------------
void AgStackLayout::Layout() {
    auto curr = Current();
    if (curr) {
        curr->SetBound(GetLocalBound());
    }
}

void AgStackLayout::Event(AgEvent& event) {
    auto curr = Current();
    if (curr) {
        curr->Event(event);
    }
}

void AgStackLayout::Draw(AgPainter& painter) {
    auto curr = Current();
    if (curr) {
        curr->Draw(painter);
    }
}

// ---------------------------------------- public ----------------------------------------

void AgStackLayout::Push(AgObj& obj) {
    if (&obj == Current().ptr) {
        return;
    }
    auto old = _ChangeObj(obj);
    if (old) {
        stack_.PushBack(*old);
    }
    ReDraw();
}

void AgStackLayout::Push2(AgObj& obj) {
    if (&obj == Current().ptr) {
        return;
    }
    /* 在栈中查找 */
    /* 未找到 */
    if (!stack_.Contain(obj)) {
        Push(obj);
        return;
    }
    /* 找到，从栈中删除 */
    stack_.Remove(obj);
    Push(obj);
}

AgList AgStackLayout::Push3(AgObj& obj) {
    if (&obj == Current().ptr) {
        return {};
    }
    /* 在栈中查找 */
    /* 未找到 */
    if (!stack_.Contain(obj)) {
        Push(obj);
        return {};
    }
    /* 找到，从栈中删除 */
    AgList ret = stack_.Cut(obj);
    ret.PopFront();
    auto old = _ChangeObj(obj);
    if (NULL != old) {
        Push(*old->As<AgObj>());
    }
    return ret;
}


NullablePtr<AgObj> AgStackLayout::Pop() {
    auto replace = stack_.PopBack();
    if (replace) {
        return _ChangeObj(*replace->As<AgObj>());
    }
    else {
        return _ChangeObj(nullptr);
    }
}

NullablePtr<AgObj> AgStackLayout::Current() {
    return FirstChild();
}

}
