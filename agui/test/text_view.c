#include "text_view.h"
#include "agui/port/draws_impl.h"

// ---------------------------------------- declare ----------------------------------------
static void TextView_Layout(AgObj* obj);
static void TextView_Draw(AgObj* obj, AgPainter* painter);
static const AgObjVFunc vfun = {
    .draw = TextView_Draw,
    .layout = TextView_Layout,
};

// ---------------------------------------- private ----------------------------------------
static void TextView_Layout(AgObj* obj) {}
static void TextView_Draw(AgObj* obj, AgPainter* painter) {
    TextView* tv = AGUI_CONTAINER_OF(TextView, obj, obj);
    TextDraw draw = {
        .font_size = tv->font_size,
        .text = tv->text,
        .x = 0,
        .y = 0,
    };
    TextDraw_Init(&draw, painter);
    painter->set_color(painter, tv->color);
    painter->call_draw(painter, &draw.draw);
}

// ---------------------------------------- public ----------------------------------------
void TextView_Init(TextView* tv) {
    AgObj_Init(&tv->obj);
    tv->obj.vfunc = vfun;
}