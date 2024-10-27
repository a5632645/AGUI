/*
 * \file ag_mq.h
 * \brief 消息队列
*/

#pragma once
#include "agui/ag_types.hpp"
#include "agui/ag_list.hpp"
#include "ag_impl/ag_impl.hpp"
#include <functional>

namespace agui {

// ---------------------------------------- AgMqCallObj ----------------------------------------
struct AgMqCallObj : public AgListNode {
    virtual void Call() = 0;
};

// ---------------------------------------- AgMq ----------------------------------------
class AgMq final {
public:
    AgMq();
    ~AgMq();

    void CallPending();

    void Enqueue(void(*call)(void*), void* arg); // c like api
    void Enqueue(std::function<void()> call); // c++ like api
    void Enqueue(AgMqCallObj* obj); // will be deleted in CallPending

private:
    impl::ag_lock_t lock_;
    AgList pending_;
    AgList swap_;
};

}
