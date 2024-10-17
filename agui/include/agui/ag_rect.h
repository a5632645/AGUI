#pragma once
#include "agui/ag_types.h"

typedef struct {
    ag_int16 x;
    ag_int16 y;
    ag_int16 w;
    ag_int16 h;
} AgRect;

/**
 * @brief 清零一个矩形
 * @param rect 
 */
void AgRect_Zero(AgRect* rect);

/**
 * @brief 判断两个矩形是否相等
 * @param rect1 
 * @param rect2 
 * @return 
 */
ag_bool AgRect_Equal(const AgRect* rect1, const AgRect* rect2);

/**
 * @brief 判断两个矩形是否相交
 * @param rect1 
 * @param rect2 
 * @return 
 */
ag_bool AgRect_IsIntersected(const AgRect* rect1, const AgRect* rect2);

/**
 * @brief 复制矩形
 * @param dest 不能为NULL
 * @param src 不能为NULL
 */
void AgRect_Copy(AgRect* dest, const AgRect* src);

/**
 * @brief 扩大或者缩小矩形
 * @param rect 不能为NULL
 * @param dx left-> <-right
 * @param dy top   ↓
 *           bootom↑
 */
void AgRect_Expand(AgRect* rect, ag_int16 dx, ag_int16 dy);