#pragma once
#include "agui/ag_types.h"

typedef struct {
    ag_int16 x;
    ag_int16 y;
    ag_int16 w;
    ag_int16 h;
} AgRect;

void AgRect_Zero(AgRect* rect);
ag_bool AgRect_Equal(const AgRect* rect1, const AgRect* rect2);