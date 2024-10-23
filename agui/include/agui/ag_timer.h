/*
 * @file agui/ag_timer.h
 * @brief 软件定时器
*/

#pragma once
#include "agui/ag_types.h"
#include "agui/ag_list.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __AgTimer {
    /* interface */
    AgListNode node;
    /* data */
    void(*timeout)(struct __AgTimer* timer); /* 不允许添加timer */
    ag_uint32 count;
    ag_uint32 period;
    ag_bool repeat : 1;
    ag_bool running : 1;
} AgTimer;

void AgTimer_Init(AgTimer* timer);
void AgTimer_Add(AgTimer* timer, AgTimer* timer2);
void AgTimer_Tick(AgTimer* timer, ag_uint32 escape);
void AgTimer_Remove(AgTimer* timer);
void AgTimer_Pause(AgTimer* timer);
void AgTimer_Resume(AgTimer* timer);
void AgTimer_Destory(AgTimer* timer);

#ifdef __cplusplus
}
#endif