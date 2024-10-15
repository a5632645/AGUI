#include "ag_comp/button.h"

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
        .rect = obj->local_bound,
        .color = AG_COLOR_GRAY,
    };
    FillDraw_Init(&fill, painter);
    painter->call_draw(painter, &fill.draw);

    /* 边框 */
    RectDraw rect = {
        .rect = obj->local_bound,
        .color = top_left
    };
    RectDraw_Init(&rect, painter);
    painter->call_draw(painter, &rect.draw);

    LineDraw line = {
        .x1 = obj->local_bound.x + obj->local_bound.w ,
        .y1 = obj->local_bound.y,
        .x2 = obj->local_bound.x + obj->local_bound.w,
        .y2 = obj->local_bound.y + obj->local_bound.h,
        .color = bottom_right
    };
    LineDraw_Init(&line, painter);
    painter->call_draw(painter, &line.draw);

    line.x1 = obj->local_bound.x;
    line.y1 = line.y2;
    painter->call_draw(painter, &line.draw);

    /* 文字 */
    TextDraw text = {
        .color = btn->color,
        .align = eAgAlign_Center,
        .font_size = btn->font_size,
        .text = btn->text,
        .rect = &obj->local_bound
    };
    TextDraw_Init(&text, painter);
    painter->call_draw(painter, &text.draw);
}

// ---------------------------------------- public ----------------------------------------
void AgButton_Init(AgButton* obj) {
    AgObj_Init(&obj->obj);
    obj->press = ag_false;
    obj->obj.vfunc.draw = _Draw;
}

void AgButton_SetPress(AgButton* btn, ag_bool press) {
    btn->press = press;
    AgObj_MarkRedraw(&btn->obj);
}