#include "agui/ag_ref_obj.h"
#include "ag_impl/mem_impl.h"
#include "ag_impl/atomic_impl.h"

// ---------------------------------------- private ----------------------------------------

typedef struct {
    ag_atomic_bool_t master_dead;
    ag_atomic_int_t ref_count;
} RefCounter;

static void _DecreseCount(RefCounter* rc) {
    AG_ATOMIC_INT_SUB(&rc->ref_count, 1);
    if (AG_ATOMIC_INT_LOAD(&rc->ref_count) == 0) {
        AgImpl_MemFree(rc);
    }
}

// ---------------------------------------- public ----------------------------------------

void AgMasterRefObj_Init(AgMasterRefObj* obj) {
    obj->ptr = AgImpl_MemAlloc(sizeof(RefCounter));
    ((RefCounter*)obj->ptr)->ref_count = 1;
    ((RefCounter*)obj->ptr)->master_dead = ag_false;
}

void AgMasterRefObj_Destory(AgMasterRefObj* obj) {
    RefCounter* rc = (RefCounter*)obj->ptr;
    AG_ATOMIC_INT_STORE(&rc->master_dead, ag_true);
    _DecreseCount(rc);
}

ag_bool AgSlaveRefObj_IsMasterDead(AgSlaveRefObj* obj) {
    RefCounter* rc = (RefCounter*)obj->ptr;
    return AG_ATOMIC_BOOL_LOAD(&rc->master_dead);
}

void AgSlaveRefObj_Init(AgSlaveRefObj* obj, AgMasterRefObj* master) {
    obj->ptr = master->ptr;
    RefCounter* rc = (RefCounter*)obj->ptr;
    AG_ATOMIC_INT_ADD(&rc->ref_count, 1);
}

void AgSlaveRefObj_Destory(AgSlaveRefObj* obj) {
    RefCounter* rc = (RefCounter*)obj->ptr;
    _DecreseCount(rc);
}