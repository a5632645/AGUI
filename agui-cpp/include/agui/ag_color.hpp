#pragma once
#include "agui/ag_types.hpp"

namespace agui {

struct AgColor {
    ag_uint8 r;
    ag_uint8 g;
    ag_uint8 b;
    ag_uint8 a;

    constexpr bool operator==(const AgColor& other) const {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }
};

namespace colors {

static const AgColor kBlack = {0, 0, 0, 255};
static const AgColor kWhite = {255, 255, 255, 255};
static const AgColor kGray = {127, 127, 127, 255};

}

}
