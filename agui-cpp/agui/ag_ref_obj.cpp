#include "agui/ag_ref_obj.hpp"
#include "ag_impl/mem_impl.hpp"
#include "ag_impl/atomic_impl.hpp"

namespace agui {

// ---------------------------------------- private ----------------------------------------

typedef struct {
    ag_atomic_bool_t master_dead;
    ag_atomic_uint8_t ref_count;
} RefCounter;

static void _DecreseCount(RefCounter* rc) {
    if (ag_atomic_uint8_dec(rc->ref_count) == 0) {
        AgImpl_MemFree(rc);
    }
}

// ---------------------------------------- public ----------------------------------------

AgMasterRefObj::AgMasterRefObj() {
    ptr = AgImpl_MemAlloc(sizeof(RefCounter));
    ((RefCounter*)ptr)->ref_count = 1;
    ((RefCounter*)ptr)->master_dead = ag_false;
}

AgMasterRefObj::~AgMasterRefObj() {
    RefCounter* rc = (RefCounter*)ptr;
    ag_atomic_bool_store(rc->master_dead, ag_true);
    _DecreseCount(rc);
}

ag_bool AgSlaveRefObj::IsMasterDead() const {
    RefCounter* rc = (RefCounter*)ptr;
    return ag_atomic_bool_load(rc->master_dead);
}

AgSlaveRefObj::AgSlaveRefObj(AgMasterRefObj& master) {
    ptr = master.ptr;
    RefCounter* rc = (RefCounter*)ptr;
    ag_atomic_uint8_inc(rc->ref_count);
}

AgSlaveRefObj::~AgSlaveRefObj() {
    RefCounter* rc = (RefCounter*)ptr;
    _DecreseCount(rc);
}

}
