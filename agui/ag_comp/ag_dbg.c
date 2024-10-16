#include "ag_comp/ag_dbg.h"

void AgDbg_DrawFrame(AgObj* obj, AgPainter* painter) {
    #if AGUI_DBG_WRIE_FRAME == 1
    RectDraw draw = {
        .color = AG_COLOR_WHITE
    };
    RectDraw_Init(&draw, painter);
    AgObj_GetLocalBound(obj, &draw.rect);
    painter->call_draw(painter, &draw.draw);
    #endif
}

#if AGUI_DBG_WRIE_FRAME == 1
void _AgObj_Draw(AgObj* obj, AgPainter* painter) {
    AgDbg_DrawFrame(obj, painter);
}
#endif