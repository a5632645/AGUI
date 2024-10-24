#include "ag_impl/default_impl.hpp"
#include "ag_impl/draws_impl.hpp"

namespace agui {

void __EmptyObjDraw(AgObj& obj, AgPainter& painter) {
    impl::AgFillDraw draw;
    draw.color = colors::kBlack;
    draw.rect = obj.GetBound();
    painter.CallDraw(draw);
}

void __DrawColorFrame(AgPainter& painter, const AgRect& aera, AgColor color) {
    impl::AgRectDraw draw;
    draw.rect = aera;
    draw.color = color;
    painter.CallDraw(draw);
}

}
