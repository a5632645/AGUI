#include "ag_comp/text_view.h"

// ---------------------------------------- private ----------------------------------------
static void TextView_Draw(AgObj* obj, AgPainter* painter);

static void _InitVFunc(AgObjVFunc* vfun) {
    vfun->draw = TextView_Draw;
}

// ---------------------------------------- impl ----------------------------------------
static void TextView_Draw(AgObj* obj, AgPainter* painter) {
    AgTextView* tv = AGUI_CONTAINER_OF(AgTextView, obj, obj);
    TextDraw draw = {
        .align = tv->align,
        .color = tv->color,
        .font_size = tv->font_size,
        .rect = &obj->local_bound,
        .text = tv->text
    };
    TextDraw_Init(&draw, painter);
    painter->call_draw(painter, &draw.draw);
}

// ---------------------------------------- public ----------------------------------------
void AgTextView_Init(AgTextView* tv) {
    AgObj_Init(&tv->obj);
    _InitVFunc(&tv->obj.vfunc);
}