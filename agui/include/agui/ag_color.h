#pragma once
#include "agui/ag_types.h"

typedef struct {
    ag_uint8 r;
    ag_uint8 g;
    ag_uint8 b;
    ag_uint8 a;
} AgColor;

static const AgColor AG_COLOR_BLACK = {0, 0, 0, 255};
static const AgColor AG_COLOR_WHITE = {255, 255, 255, 255};