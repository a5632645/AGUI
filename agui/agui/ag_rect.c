#include "agui/ag_rect.h"

void AgRect_Zero(AgRect* rect) {
    rect->x = 0;
    rect->y = 0;
    rect->w = 0;
    rect->h = 0;
}

ag_bool AgRect_Equal(const AgRect* rect1, const AgRect* rect2) {
    if (rect1->x != rect2->x) {
        return ag_false;
    }
    if (rect1->y != rect2->y) {
        return ag_false;
    }
    if (rect1->w != rect2->w) {
        return ag_false;
    }
    if (rect1->h != rect2->h) {
        return ag_false;
    }
    return ag_true;
}

ag_bool AgRect_IsIntersected(const AgRect* rect1, const AgRect* rect2) {
    // 矩形1的边界
    ag_int16 rect1_left = rect1->x;
    ag_int16 rect1_right = rect1->x + rect1->w;
    ag_int16 rect1_top = rect1->y;
    ag_int16 rect1_bottom = rect1->y + rect1->h;

    // 矩形2的边界
    ag_int16 rect2_left = rect2->x;
    ag_int16 rect2_right = rect2->x + rect2->w;
    ag_int16 rect2_top = rect2->y;
    ag_int16 rect2_bottom = rect2->y + rect2->h;

    // 检查是否相交
    return !(rect1_right <= rect2_left || 
             rect1_left >= rect2_right || 
             rect1_bottom <= rect2_top || 
             rect1_top >= rect2_bottom);
}