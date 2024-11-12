#include "agui/ag_timer.hpp"

namespace agui {

void AgTimer::AddTimer(AgTimer& timer2) {
    if (nullptr != AgTimer::AgListNode::next_) {
        AgTimer::AgListNode::next_->AgTimer::AgListNode::prev_ = &timer2;
    }
    timer2.AgTimer::AgListNode::next_ = AgTimer::AgListNode::next_;
    timer2.AgTimer::AgListNode::prev_ = this;
    AgTimer::AgListNode::next_ = &timer2;
}

void AgTimer::Tick(ag_uint32 escape) {
    AgListNode* node = this;
    while (nullptr != node) {
        AgListNode* next = node->AgTimer::AgListNode::next_;
        AgTimer* t = next->As<AgTimer>();
        if (ag_true == t->running_) {
            t->count_ += escape;
            if (t->count_ >= t->period_) {
                while (t->count_ >= t->period_) {
                    t->count_ -= t->period_;
                }
                if (ag_false == t->repeat_) {
                    t->Remove();
                }                                                                                                                                                     
                t->Timeout();
            }
        }
        node = next;
    }
}

void AgTimer::Remove() {
    AgListNode* prev = AgTimer::AgListNode::prev_;
    AgListNode* next = AgTimer::AgListNode::next_;
    if (nullptr != prev) {
        prev->AgTimer::AgListNode::next_ = next;
    }
    if (nullptr != next) {
        next->AgTimer::AgListNode::prev_ = prev;
    }
    AgTimer::AgListNode::prev_ = nullptr;
    AgTimer::AgListNode::next_ = nullptr;
}

void AgTimer::Pause() {
    running_ = ag_false;
}

void AgTimer::Resume() {
    running_ = ag_true;
}

void AgTimer::Destory() {
    AgListNode* node = this;
    while (nullptr != node) {
        AgListNode* next = node->AgTimer::AgListNode::next_;
        node->AgTimer::AgListNode::prev_ = nullptr;
        node->AgTimer::AgListNode::next_ = nullptr;
        node = next;
    }
}

NullablePtr<AgTimer> AgTimer::NextTimer() {
    if (nullptr == AgTimer::AgListNode::next_) {
        return nullptr;
    }
    return AgTimer::AgListNode::next_->As<AgTimer>();
}

NullablePtr<AgTimer> AgTimer::PrevTimer() {
    if (nullptr == AgTimer::AgListNode::prev_) {
        return nullptr;
    }
    return AgTimer::AgListNode::prev_->As<AgTimer>();
}

}

