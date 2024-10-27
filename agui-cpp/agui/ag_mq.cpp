#include "agui/ag_mq.hpp"
#include "ag_impl/ag_impl.hpp"

namespace agui {

// ---------------------------------------- lock_guard ----------------------------------------
struct LockGuard {
    impl::ag_lock_t& lock_;
    LockGuard(impl::ag_lock_t& lock) : lock_(lock) {
        impl::AgImpl_Lock(lock_);
    }
    ~LockGuard() {
        impl::AgImpl_Unlock(lock_);
    }
};

// ---------------------------------------- AgMq ----------------------------------------

AgMq::AgMq() {
    impl::AgImpl_LockInit(lock_);
}

AgMq::~AgMq() {
    CallPending();
    impl::AgImpl_LockDestroy(lock_);
}

void AgMq::Enqueue(void(*call)(void*), void* arg) {
    struct CallObj : public AgMqCallObj {
        void(*call_)(void*);
        void* arg_;
        void Call() override {
            call_(arg_);
        }
        CallObj(void(*call)(void*), void* arg) : call_(call), arg_(arg) {}
    };
    AgMqCallObj* obj = new(std::nothrow) CallObj(call, arg);
    {
        LockGuard {lock_};
        pending_.PushBack(*obj);
    }
}

void AgMq::Enqueue(std::function<void()> call) {
    struct CallObj : public AgMqCallObj {
        std::function<void()> call_;
        void Call() override {
            call_();
        }
        CallObj(std::function<void()> call) : call_(std::move(call)) {}
    };
    AgMqCallObj* obj = new(std::nothrow) CallObj(std::move(call));
    {
        LockGuard {lock_};
        pending_.PushBack(*obj);
    }
}

void AgMq::Enqueue(AgMqCallObj* obj) {
    LockGuard {lock_};
    pending_.PushBack(*obj);
}

void AgMq::CallPending() {
    {
        LockGuard {lock_};
        swap_.Append(pending_);
    }
    swap_.RemoveAll([](AgListNode* node) {
        AgMqCallObj* obj = node->As<AgMqCallObj>();
        obj->Call();
        delete obj;
    });
}

}
