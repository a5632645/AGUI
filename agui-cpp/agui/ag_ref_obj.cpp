#include "agui/ag_ref_obj.hpp"
#include "ag_impl/ag_impl.hpp"

namespace agui {

// ---------------------------------------- private ----------------------------------------

typedef struct {
    impl::ag_atomic_bool_t master_dead;
    impl::ag_atomic_uint8_t ref_count;
} RefCounter;

static void _DecreseCount(RefCounter* rc) {
    if (impl::ag_atomic_uint8_dec(rc->ref_count) == 0) {
        rc->~RefCounter();
        impl::AgImpl_MemFree(rc);
    }
}

// ---------------------------------------- public ----------------------------------------

AgMasterRefObj::AgMasterRefObj() {
    ptr = impl::AgImpl_MemAlloc(sizeof(RefCounter));
    new(ptr) RefCounter{};
    ((RefCounter*)ptr)->ref_count = 1;
    ((RefCounter*)ptr)->master_dead = ag_false;
}

AgMasterRefObj::~AgMasterRefObj() {
    DestroyMaster();
}

AgMasterRefObj::AgMasterRefObj(AgMasterRefObj&& other) noexcept {
    DestroyMaster();
    ptr = other.ptr;
    other.ptr = nullptr;
}

AgMasterRefObj& AgMasterRefObj::operator=(AgMasterRefObj&& other) noexcept {
    DestroyMaster();
    ptr = other.ptr;
    other.ptr = nullptr;
    return *this;
}

void AgMasterRefObj::DestroyMaster() {
    RefCounter* rc = (RefCounter*)ptr;
    impl::ag_atomic_bool_store(rc->master_dead, ag_true);
    _DecreseCount(rc);
}

AgSlaveRefObj::AgSlaveRefObj(AgSlaveRefObj&& other) noexcept {
    DestroySlave();
    ptr = other.ptr;
    other.ptr = nullptr;
}

AgSlaveRefObj& AgSlaveRefObj::operator=(AgSlaveRefObj&& other) noexcept {
    DestroySlave();
    ptr = other.ptr;
    other.ptr = nullptr;
    return *this;
}

ag_bool AgSlaveRefObj::IsMasterDead() const {
    if (nullptr == ptr) {
        return ag_true;
    }
    RefCounter* rc = (RefCounter*)ptr;
    return impl::ag_atomic_bool_load(rc->master_dead);
}

void AgSlaveRefObj::DestroySlave() {
    RefCounter* rc = (RefCounter*)ptr;
    _DecreseCount(rc);
}

AgSlaveRefObj::AgSlaveRefObj(AgMasterRefObj& master) {
    ptr = master.ptr;
    RefCounter* rc = (RefCounter*)ptr;
    impl::ag_atomic_uint8_inc(rc->ref_count);
}

AgSlaveRefObj::~AgSlaveRefObj() {
    DestroySlave();
}

}
