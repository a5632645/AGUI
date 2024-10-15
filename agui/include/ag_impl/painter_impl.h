#pragma once
#include "agui/ag_painter.h"
#include "raylib.h"

typedef struct {
    AgPainter painter;
} PainterImpl;

void PainterImpl_Init(PainterImpl* impl);