#include "agui/ag_rect.hpp"

namespace agui {

constexpr void AgRect::Zero() {
    x = 0;
    y = 0;
    w = 0;
    h = 0;
}

constexpr ag_bool AgRect::IsIntersected(const AgRect& rect2) const {
    // 矩形1的边界
    ag_int16 rect1_left = x;
    ag_int16 rect1_right = x + w;
    ag_int16 rect1_top = y;
    ag_int16 rect1_bottom = y + h;

    // 矩形2的边界
    ag_int16 rect2_left = rect2.x;
    ag_int16 rect2_right = rect2.x + rect2.w;
    ag_int16 rect2_top = rect2.y;
    ag_int16 rect2_bottom = rect2.y + rect2.h;

    // 检查是否相交
    return !(rect1_right <= rect2_left || 
             rect1_left >= rect2_right || 
             rect1_bottom <= rect2_top || 
             rect1_top >= rect2_bottom);
}

constexpr void AgRect::Expand(ag_int16 dx, ag_int16 dy) {
    x -= dx;
    y -= dy;
    w += dx * 2;
    h += dy * 2;
}

}
