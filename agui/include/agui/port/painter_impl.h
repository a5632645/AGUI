#include "agui/ag_painter.h"
#include "raylib.h"

typedef struct {
    AgPainter painter;
    Color color;
} PainterImpl;

void PainterImpl_Init(PainterImpl* impl);