#pragma once
#include "agui/ag_types.hpp"

namespace agui {

enum class AgAlignEnum : ag_uint8 {
    kXLeft = 0b01,
    kXCenter = 0b10,
    kXRight = 0b11,

    kYTop = 0b0100,
    kYCenter = 0b1000,
    kYBottom = 0b1100,

    kCenter = 0b1010,
    eAgAlign_None = 0,
};

static constexpr AgAlignEnum AGUI_X_ALIGN(AgAlignEnum xx) {
    auto x = static_cast<ag_uint8>(xx);
    return static_cast<AgAlignEnum>(x & 0b11);
}
static constexpr AgAlignEnum AGUI_Y_ALIGN(AgAlignEnum yy) {
    auto y = static_cast<ag_uint8>(yy);
    return static_cast<AgAlignEnum>(((y) & 0b1100)); 
}

}