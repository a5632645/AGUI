#include "agui/ag_mq.h"
#include "ag_impl/mem_impl.h"
#include "ag_impl/lock_impl.h"

void AgMqCallObj_Init(AgMqCallObj* obj) {
    AgListNode_Init(&obj->node);
    obj->call = NULL;
    obj->arg = NULL;
}

void AgMq_Init(AgMq* mq) {
    AgList_Init(&mq->pending);
    AgList_Init(&mq->frees);
    mq->lock = AgImpl_LockCreate();
}

void AgMq_CallPending(AgMq* mq) {
    AgImpl_Lock(mq->lock);
    if (AgList_IsEmpty(&mq->pending)) {
        AgImpl_Unlock(mq->lock);
        return;
    }
    AgList tmp;
    AgList_Cut(&mq->pending, &tmp, mq->pending.head);
    AgImpl_Unlock(mq->lock);

    /* 离开临界区 */
    AgListNode* node = tmp.head;
    while (NULL != node) {
        AgMqCallObj* obj = AGUI_CONTAINER_OF(AgMqCallObj, node, node);
        obj->call(obj->arg);
        node = node->next;
    }
    /* 离开临界区 */

    AgImpl_Lock(mq->lock);
    AgList_Append(&mq->frees, &tmp);
    AgImpl_Unlock(mq->lock);
}

ag_bool AgMq_Post(AgMq* mq, void(*call)(void* arg), void* arg) {
    AgImpl_Lock(mq->lock);

    AgListNode* node = AgList_Popfront(&mq->frees);
    if (NULL == node) {
        node = AgImpl_MemAlloc(sizeof(AgMqCallObj));
        if (NULL == node) {
            AgImpl_Unlock(mq->lock);
            return ag_false;
        }
        AgMqCallObj* obj = AGUI_CONTAINER_OF(AgMqCallObj, node, node);
        AgMqCallObj_Init(obj);
    }
    AgMqCallObj* obj = AGUI_CONTAINER_OF(AgMqCallObj, node, node);
    obj->call = call;
    obj->arg = arg;
    AgList_PushBack(&mq->pending, node);

    AgImpl_Unlock(mq->lock);
    return ag_true;
}

void AgMq_Destroy(AgMq* mq) {
    AgMq_CallPending(mq);
    AgListNode* node = mq->frees.head;
    while (NULL != node) {
        AgListNode* next = node->next;
        AgImpl_MemFree(node);
        node = next;
    }
}
