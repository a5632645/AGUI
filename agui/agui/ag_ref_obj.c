#include "agui/ag_ref_obj.h"
#include "ag_impl/lock_impl.h"

// ---------------------------------------- AgRefObj ----------------------------------------
ag_bool AgRefObj_IsMasterDead(AgRefObj* obj) {
    if (NULL == obj) {
        return ag_true;
    }
    return AG_ATOMIC_BOOL_LOAD(&obj->master_dead);
}

// ---------------------------------------- AgMasterRefObj ----------------------------------------
void AgMasterRefObj_Init(AgMasterRefObj* obj) {
    if (NULL == obj) {
        return;
    }
    AgList_Init(&obj->refs);
    obj->dead = ag_false;
    obj->lock = AgImpl_LockCreate();
}

void AgMasterRefObj_AddRef(AgMasterRefObj* obj, AgRefObj* ref) {
    if (NULL == obj || NULL == ref) {
        return;
    }
    AgImpl_Lock(obj->lock);
    ref->master_dead = obj->dead;
    if (ag_false == obj->dead) {
        AgList_PushBack(&obj->refs, &ref->node);
    }
    AgImpl_Unlock(obj->lock);
}

void AgMasterRefObj_Remove(AgMasterRefObj* obj, AgRefObj* ref) {
    if (NULL == obj || NULL == ref) {
        return;
    }
    AgImpl_Lock(obj->lock);
    if (ag_false == AgRefObj_IsMasterDead(ref) && ag_false == obj->dead) {
        AgList_Remove(&obj->refs, &ref->node);
    }
    AgImpl_Unlock(obj->lock);
}

void AgMasterRefObj_Destroy(AgMasterRefObj* obj) {
    if (NULL == obj) {
        return;
    }

    AgImpl_Lock(obj->lock);
    obj->dead = ag_true;
    AgListNode* node = obj->refs.head;
    while (NULL != node) {
        AgListNode* next = node->next;
        AgRefObj* ref = AGUI_CONTAINER_OF(AgRefObj, node, node);
        AG_ATOMIC_BOOL_STORE(&ref->master_dead, ag_true);
        node->next = NULL;
        node->prev = NULL;
        node = next;
    }
    obj->refs.head = NULL;
    obj->refs.tail = NULL;
    AgImpl_Unlock(obj->lock);

    AgImpl_LockDestroy(obj->lock);
}
