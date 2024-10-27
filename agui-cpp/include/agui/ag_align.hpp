#pragma once
#include "agui/ag_types.hpp"

namespace agui {

enum AgAlignEnum : ag_uint8 {
    eAgAlign_XLeft = 0b01,
    eAgAlign_XCenter = 0b10,
    eAgAlign_XRight = 0b11,

    eAgAlign_YTop = 0b0100,
    eAgAlign_YCenter = 0b1000,
    eAgAlign_YBottom = 0b1100,

    eAgAlign_Center = eAgAlign_XCenter | eAgAlign_YCenter,
    eAgAlign_None = 0,
};

static constexpr AgAlignEnum AGUI_X_ALIGN(ag_uint8 x) { return static_cast<AgAlignEnum>(x & 0b11); }
static constexpr AgAlignEnum AGUI_Y_ALIGN(ag_uint8 y) { return static_cast<AgAlignEnum>(((y) & 0b1100)); }

}