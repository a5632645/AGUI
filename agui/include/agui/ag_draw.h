#pragma once
#include "agui/ag_types.h"
#include "agui/ag_rect.h"

typedef struct {
    ag_uint32 type;
    const AgRect* clip;
} AgDraw;