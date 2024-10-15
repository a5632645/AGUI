#pragma once
#include "ag_comp/deps.h"

typedef struct {
    /* interface */
    AgObj obj;
    /* data */
    ag_bool press;
    const char* text;
    AgColor color;
    ag_uint32 font_size;
} AgButton;

void AgButton_Init(AgButton* obj);
void AgButton_SetPress(AgButton* btn, ag_bool press);