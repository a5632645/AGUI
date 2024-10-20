#include "agui/ag_timer.h"

void AgTimer_Init(AgTimer* timer) {
    AgListNode_Init(&timer->node);
    timer->timeout = NULL;
    timer->count = 0;
    timer->period = 0;
    timer->repeat = ag_false;
    timer->running = ag_true;
}

void AgTimer_Add(AgTimer* timer, AgTimer* timer2) {
    if (NULL != timer->node.next) {
        timer->node.next->prev = &timer2->node;
    }
    timer2->node.next = timer->node.next;
    timer2->node.prev = &timer->node;
    timer->node.next = &timer2->node;
}

void AgTimer_Tick(AgTimer* timer, ag_uint32 escape) {
    AgListNode* node = &timer->node;
    while (NULL != node) {
        AgTimer* t = AGUI_CONTAINER_OF(AgTimer, node, node);
        AgListNode* next = node->next;
        if (ag_true == t->running) {
            t->count += escape;
            if (t->count >= t->period) {
                while (t->count >= t->period) {
                    t->count -= t->period;
                }
                if (ag_false == t->repeat) {
                    AgTimer_Remove(t);
                }
                t->timeout(t);
            }
        }
        node = next;
    }
}

void AgTimer_Remove(AgTimer* timer) {
    AgListNode* prev = timer->node.prev;
    AgListNode* next = timer->node.next;
    if (NULL != prev) {
        prev->next = next;
    }
    if (NULL != next) {
        next->prev = prev;
    }
    timer->node.prev = NULL;
    timer->node.next = NULL;
}

void AgTimer_Pause(AgTimer* timer) {
    timer->running = ag_false;
}

void AgTimer_Resume(AgTimer* timer) {
    timer->running = ag_true;
}

void AgTimer_Destory(AgTimer* timer) {
    AgListNode* node = &timer->node;
    while (NULL != node) {
        AgListNode* next = node->next;
        node->prev = NULL;
        node->next = NULL;
        node = next;
    }
}
