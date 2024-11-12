#include "ag_comp/ag_stack_layout.hpp"

namespace agui {

// ---------------------------------------- private ----------------------------------------
NullablePtr<AgObj> AgStackLayout::Exchange(AgObj& obj) {
    auto curr = Current();
    if (curr) {
        curr->LeaveParent();
    }
    AddChild(obj);
    Layout();
    ReDraw();
    return curr;
}

NullablePtr<AgObj> AgStackLayout::Exchange(NullablePtr<AgObj> obj) {
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
    return Exchange(*obj);
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

// ---------------------------------------- public ----------------------------------------

void AgStackLayout::Push(AgObj& obj) {
    if (&obj == Current().ptr) {
        return;
    }
    auto old = Exchange(obj);
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
    auto old = Exchange(obj);
    if (NULL != old) {
        Push(*old->As<AgObj>());
    }
    return ret;
}


NullablePtr<AgObj> AgStackLayout::Pop() {
    auto replace = stack_.PopBack();
    if (replace) {
        return Exchange(*replace->As<AgObj>());
    }
    else {
        return Exchange(nullptr);
    }
}

NullablePtr<AgObj> AgStackLayout::Current() {
    return FirstChild();
}

}
