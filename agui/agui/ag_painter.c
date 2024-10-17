#include "agui/ag_painter.h"

// ---------------------------------------- public ----------------------------------------
void AgPainter_Init(AgPainter* painter) {
    painter->get_backend = NULL;
    painter->call_draw = NULL;
    painter->begin_frame = NULL;
    painter->end_frame = NULL;
    painter->always_redraw = ag_false;
    painter->depth = -1;
    AgRect_Zero(&painter->draw_aera);
}

const AgRect* AgPainter_GetDrawAera(AgPainter* painter) {
    return &painter->data[painter->depth].draw_aera;
}

const AgRect* AgPainter_GetClipAera(AgPainter* painter) {
    if (painter->depth == 0) {
        return &painter->data[0].draw_aera;
    }
    else {
        return &painter->data[painter->depth - 1].draw_aera;
    }
}

void AgPainter_SaveState(AgPainter* painter) {
    struct __PainterData* data = &painter->data[++painter->depth];
    AgRect_Copy(&data->draw_aera, &painter->draw_aera);
    data->always_redraw = painter->always_redraw;
}

void AgPainter_RestoreState(AgPainter* painter) {
    painter->always_redraw = painter->data[painter->depth].always_redraw;
    AgRect_Copy(&painter->draw_aera, &painter->data[painter->depth].draw_aera);
    --painter->depth;
}

void AgPainter_PrepareDraw(AgPainter* painter, const AgRect* global_aera) {
    AgRect_Copy(&painter->draw_aera, global_aera);
    painter->always_redraw = ag_false;
    painter->depth = -1;
}