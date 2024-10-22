#include "ag_impl/default_impl.h"
#include "ag_impl/draws_impl.h"

void __EmptyObjDraw(AgObj* obj, AgPainter* painter) {
    AgFillDraw draw;
    AgFillDraw_Init(&draw);
    AgObj_GetLocalBound(obj, &draw.rect);
    draw.color = AG_COLOR_BLACK;
    painter->call_draw(painter, &draw.draw);
}

void __DrawColorFrame(AgPainter* painter, const AgRect* aera, AgColor color) {
    AgRectDraw draw;
    AgRectDraw_Init(&draw);
    draw.rect = *aera;
    draw.color = color;
    painter->call_draw(painter, &draw.draw);
}