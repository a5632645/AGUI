/*
 * \file ag_stack_layout.h
 * \brief 可堆叠的布局，出栈入栈
*/

#pragma once
#include "agui/ag_obj.hpp"

namespace agui {

class AgStackLayout : public AgObj {
public:
    AgStackLayout(AgObj* parent = nullptr) : AgObj(parent) {}

    /**
     * @brief 直接推入栈，不要与已有的重复
     * @param obj 引用
     */
    void Push(AgObj& obj);

    /**
     * @brief 如果栈中存在，将其提升到顶层
     * @param obj 
     */
    void Push2(AgObj& obj);

    /**
     * @brief 如果栈中存在，弹出其上方的所以OBJ引用
     * @param obj 
     * @return 
     */
    AgList Push3(AgObj& obj);

    /**
     * @brief 交换当前正在显示的OBJ
     * @param obj 
     * @return 
     */
    NullablePtr<AgObj> Exchange(AgObj& obj);

    /**
     * @brief 交换当前正在显示的OBJ
     * @param obj 
     * @return 
     */
    NullablePtr<AgObj> Exchange(NullablePtr<AgObj> obj);

    /**
     * @brief 弹出正在显示的OBJ
     * @return 
     */
    NullablePtr<AgObj> Pop();

    /**
     * @brief 获取正在显示的OBJ
     * @return 
     */
    NullablePtr<AgObj> Current();

    void Layout() override;
    void Event(AgEvent& event) override;
private:
    AgList stack_;
};

}
