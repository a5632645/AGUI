#include "ag_comp/ag_dbg.h"

void AgDbg_DrawFrame(AgObj* obj, AgPainter* painter) {
    #if AGUI_DBG_WRIE_FRAME == 1
    AgRectDraw draw = {
        .color = AG_COLOR_WHITE
    };
    AgRectDraw_Init(&draw, painter);
    AgObj_GetLocalBound(obj, &draw.rect);
    painter->call_draw(painter, &draw.draw);
    #endif
}
