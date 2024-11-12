#pragma once
#include "agui/ag_types.hpp"

namespace agui {

struct AgRect {
    ag_int16 x{};
    ag_int16 y{};
    ag_int16 w{};
    ag_int16 h{};

    constexpr void Zero() {
        x = 0;
        y = 0;
        w = 0;
        h = 0;
    }

    constexpr ag_bool operator==(const AgRect& other) const {
        return x == other.x && y == other.y && w == other.w && h == other.h;
    }

    constexpr ag_bool IsIntersected(const AgRect& rect2) const {
        // ����1�ı߽�
        ag_int16 rect1_left = x;
        ag_int16 rect1_right = x + w;
        ag_int16 rect1_top = y;
        ag_int16 rect1_bottom = y + h;

        // ����2�ı߽�
        ag_int16 rect2_left = rect2.x;
        ag_int16 rect2_right = rect2.x + rect2.w;
        ag_int16 rect2_top = rect2.y;
        ag_int16 rect2_bottom = rect2.y + rect2.h;

        // ����Ƿ��ཻ
        return !(rect1_right <= rect2_left ||
                 rect1_left >= rect2_right ||
                 rect1_bottom <= rect2_top ||
                 rect1_top >= rect2_bottom);
    }

    constexpr void Expand(ag_int16 dx, ag_int16 dy) {
        x -= dx;
        y -= dy;
        w += dx * 2;
        h += dy * 2;
    }
};

}
