#pragma once
#include "ag_comp/deps.h"

typedef struct {
    /* interface */
    AgObj obj;
    /* data */
    const char* text;
    AgColor color;
    uint32_t font_size;
    AgAlignEnum align;
} AgTextView;

void AgTextView_Init(AgTextView* tv);