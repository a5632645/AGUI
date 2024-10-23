/*
 * @file agui/ag_timer.h
 * @brief 软件定时器
*/

#pragma once
#include "agui/ag_types.hpp"
#include "agui/ag_list.hpp"

namespace agui {

class AgTimer : public AgListNode {
public:
    virtual ~AgTimer() = default;
    virtual void Timeout() = 0;

    void AddTimer(AgTimer& other);
    void Tick(ag_uint32 escape);
    void Remove();
    void Pause();
    void Resume();
    void Destory();
private:
    ag_uint32 count_{};
    ag_uint32 period_{};
    ag_bool repeat_ : 1 = ag_true;
    ag_bool running_ : 1 = ag_false;
};

}