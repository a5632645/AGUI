#include "agui/ag_timer.hpp"

namespace agui {

void AgTimer::AddTimer(AgTimer& timer2) {
    if (NULL != next_) {
        next_->prev_ = &timer2;
    }
    timer2.next_ = next_;
    timer2.prev_ = this;
    next_ = &timer2;
}

void AgTimer::Tick(ag_uint32 escape) {
    AgListNode* node = this;
    while (NULL != node) {
        AgListNode* next = node->next_;
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
    AgListNode* prev = prev_;
    AgListNode* next = next_;
    if (NULL != prev) {
        prev->next_ = next;
    }
    if (NULL != next) {
        next->prev_ = prev;
    }
    prev_ = NULL;
    next_ = NULL;
}

void AgTimer::Pause() {
    running_ = ag_false;
}

void AgTimer::Resume() {
    running_ = ag_true;
}

void AgTimer::Destory() {
    AgListNode* node = this;
    while (NULL != node) {
        AgListNode* next = node->next_;
        node->prev_ = NULL;
        node->next_ = NULL;
        node = next;
    }
}

}

