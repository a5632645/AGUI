#pragma once
#include "agui/ag_types.hpp"

namespace agui {

struct AgRect {
    ag_int16 x{};
    ag_int16 y{};
    ag_int16 w{};
    ag_int16 h{};

    constexpr void Zero();
    constexpr ag_bool operator==(const AgRect& other) const;
    constexpr ag_bool IsIntersected(const AgRect& other) const;
    constexpr void Expand(ag_int16 dx, ag_int16 dy);
};

}
