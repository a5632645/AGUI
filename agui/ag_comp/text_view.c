#include "ag_comp/text_view.h"
#include "ag_comp/ag_dbg.h"

// ---------------------------------------- private ----------------------------------------
static void TextView_Draw(AgObj* obj, AgPainter* painter);

static void _InitVFunc(AgObjVFunc* vfun) {
    vfun->draw = TextView_Draw;
}

// ---------------------------------------- impl ----------------------------------------
static void TextView_Draw(AgObj* obj, AgPainter* painter) {
    AgTextView* tv = AGUI_CONTAINER_OF(AgTextView, obj, obj);
    AgTextDraw draw = {
        .align = tv->align,
        .color = tv->color,
        .font_size = tv->font_size,
        .text = tv->text
    };
    AgTextDraw_Init(&draw, painter);
    AgObj_GetLocalBound(&tv->obj, &draw.rect);
    painter->call_draw(painter, &draw.draw);

    AgDbg_DrawFrame(obj, painter);
}

// ---------------------------------------- public ----------------------------------------
void AgTextView_Init(AgTextView* tv) {
    AgObj_Init(&tv->obj);
    _InitVFunc(&tv->obj.vfunc);
    tv->obj.flags.transpant = ag_true;
    tv->obj.obj_type = eAgObjType_TextView;
    tv->align = eAgAlign_None;
    tv->color = AG_COLOR_WHITE;
    tv->font_size = 16;
    tv->text = "TextView";
}