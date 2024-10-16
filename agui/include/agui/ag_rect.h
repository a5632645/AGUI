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