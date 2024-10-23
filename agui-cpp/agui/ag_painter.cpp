#include "agui/ag_painter.hpp"

namespace agui {

const AgRect& AgPainter::GetDrawAera() const {
    return draw_aera_;
}

AgRect AgPainter::GetLocalDrawAera() const {
    AgRect draw_aera = draw_aera_;
    draw_aera.x = 0;
    draw_aera.y = 0;
    return draw_aera;   
}

void AgPainter::SaveState() {
    PainterData* data = &data[++depth_];
    data->draw_aera = draw_aera_;
    data->always_redraw = always_redraw_;
}

void AgPainter::RestoreState() {
    PainterData* data = &data_[depth_];
    always_redraw_ = data->always_redraw;
    draw_aera_ = data->draw_aera;
    --depth_;
}

void AgPainter::PrepareDraw(const AgRect& global_aera) {
    draw_aera_ = global_aera;
    always_redraw_ = ag_false;
    depth_ = -1;
}

AgRect& AgPainter::ModifyCurrDrawAera() {
    return draw_aera_;
}

}
