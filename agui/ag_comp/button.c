#include "ag_comp/button.h"
#include "ag_comp/ag_dbg.h"

// ---------------------------------------- impl ----------------------------------------
static void _Draw(AgObj* obj, AgPainter* painter) {
    AgButton* btn = AGUI_CONTAINER_OF(AgButton, obj, obj);
    AgColor top_left = AG_COLOR_WHITE;
    AgColor bottom_right = AG_COLOR_BLACK;
    if (ag_true == btn->press) {
        top_left = AG_COLOR_BLACK;
        bottom_right = AG_COLOR_WHITE;
    }

    /* 背景 */
    FillDraw fill = {
        .color = AG_COLOR_GRAY,
    };
    FillDraw_Init(&fill, painter);
    AgObj_GetLocalBound(obj, &fill.rect);
    painter->call_draw(painter, &fill.draw);
    const AgRect* local_bound = &fill.rect;

    /* 边框 */
    RectDraw rect = {
        .color = top_left
    };
    RectDraw_Init(&rect, painter);
    AgObj_GetLocalBound(obj, &rect.rect);
    painter->call_draw(painter, &rect.draw);

    LineDraw line = {
        .x1 = local_bound->x + local_bound->w ,
        .y1 = local_bound->y,
        .x2 = local_bound->x + local_bound->w,
        .y2 = local_bound->y + local_bound->h,
        .color = bottom_right
    };
    LineDraw_Init(&line, painter);
    painter->call_draw(painter, &line.draw);

    line.x1 = local_bound->x;
    line.y1 = line.y2;
    painter->call_draw(painter, &line.draw);

    /* 文字 */
    TextDraw text = {
        .color = btn->color,
        .align = eAgAlign_Center,
        .font_size = btn->font_size,
        .text = btn->text,
    };
    TextDraw_Init(&text, painter);
    AgObj_GetLocalBound(obj, &text.rect);
    painter->call_draw(painter, &text.draw);

    AgDbg_DrawFrame(obj, painter);
}

// ---------------------------------------- public ----------------------------------------
void AgButton_Init(AgButton* obj) {
    AgObj_Init(&obj->obj);
    obj->obj.obj_type = eAgObjType_Button;
    obj->obj.vfunc.draw = _Draw;
    obj->press = ag_false;
    obj->color = AG_COLOR_WHITE;
    obj->font_size = 16;
    obj->text = "button";
}

void AgButton_SetPress(AgButton* btn, ag_bool press) {
    btn->press = press;
    AgObj_Redraw(&btn->obj);
}