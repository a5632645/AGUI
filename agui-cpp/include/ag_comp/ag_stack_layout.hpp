/*
 * \file ag_stack_layout.h
 * \brief 可堆叠的布局，出栈入栈
*/

#pragma once
#include "ag_comp/deps.hpp"

namespace agui {

class AgStackLayout : public AgObj {
public:
    AgStackLayout(AgObj* parent = nullptr) : AgObj(parent, eAgObjType_StackLayout) {}

    void Push(AgObj& obj);
    void Push2(AgObj& obj);
    AgList Push3(AgObj& obj);
    NullablePtr<AgObj> Pop();
    NullablePtr<AgObj> Current();

    void Layout() override;
    void Event(AgEvent& event) override;
    void Draw(AgPainter& painter) override;
private:
    NullablePtr<AgObj> _ChangeObj(AgObj& obj);
    NullablePtr<AgObj> _ChangeObj(NullablePtr<AgObj> obj);

    AgList stack_;
};

}
