#include "ag_comp/button.h"

// ---------------------------------------- impl ----------------------------------------
static void _Draw(AgObj* obj, AgPainter* painter) {
    AgButton* btn = AGUI_CONTAINER_OF(AgButton, obj, obj);
    AgColor top_left = AG_COLOR_WHITE;
    AgColor bottom_right = AG_COLOR_BLACK;
    if (ag_true == btn->press) {
        top_left = AG_COLOR_BLACK;
        bottom_right = AG_COLOR_GRAY;
    }

    /* 背景 */
    AgFillDraw fill = {
        .color = AG_COLOR_GRAY,
    };
    AgFillDraw_Init(&fill, painter);
    AgObj_GetLocalBound(obj, &fill.rect);
    painter->call_draw(painter, &fill.draw);
    const AgRect* local_bound = &fill.rect;

    /* 边框 */
    AgRectDraw rect = {
        .color = AG_COLOR_BLACK,
        .rect = *local_bound
    };
    AgRectDraw_Init(&rect, painter);
    AgObj_GetLocalBound(obj, &rect.rect);
    painter->call_draw(painter, &rect.draw);

    /* 高亮阴影 */
    AgRect_Expand(&fill.rect, -1, -1);
    AgLineDraw line = {
        .color = top_left,
        .x1 = fill.rect.x,
        .y1 = fill.rect.y,
        .x2 = fill.rect.x + fill.rect.w,
        .y2 = fill.rect.y,
    };
    AgLineDraw_Init(&line, painter);
    painter->call_draw(painter, &line.draw);

    line.x2 = fill.rect.x;
    line.y2 = fill.rect.y + fill.rect.h;
    painter->call_draw(painter, &line.draw);

    line.color = bottom_right;
    line.x1 = fill.rect.x + fill.rect.w;
    line.y1 = fill.rect.y + fill.rect.h;
    painter->call_draw(painter, &line.draw);

    line.x2 = fill.rect.x + fill.rect.w;
    line.y2 = fill.rect.y;
    painter->call_draw(painter, &line.draw);

    /* 文字 */
    AgTextDraw text = {
        .color = btn->color,
        .align = eAgAlign_Center,
        .font_size = btn->font_size,
        .text = btn->text,
    };
    AgTextDraw_Init(&text, painter);
    AgObj_GetLocalBound(obj, &text.rect);
    painter->call_draw(painter, &text.draw);
}

// ---------------------------------------- public ----------------------------------------
void AgButton_Init(AgButton* obj) {
    AgObj_Init(&obj->obj);
    obj->obj.flags.transpant = ag_false;
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