#include "ag_impl/painter_impl.h"
#include "ag_impl/draws_impl.h"
#include "ag_comp/ag_list_view.h"
#include "raylib.h"
#include <stdio.h>

static void Lv_BackGround(AgPainter* painter, const AgRect* aera) {
    AgFillDraw draw;
    AgFillDraw_Init(&draw);
    draw.rect = *aera;
    draw.color = AG_COLOR_BLACK;
    painter->call_draw(painter, &draw.draw);
}

static void Lv_ScrollBarH(AgPainter* painter, ag_int16 x, ag_int16 x_end) {
    {
        AgFillDraw draw;
        AgFillDraw_Init(&draw);
        AgPainter_GetLocalDrawAera(painter, &draw.rect);
        draw.color = AG_COLOR_BLACK;
        painter->call_draw(painter, &draw.draw);
    }
    {
        AgRectDraw rect;
        AgRectDraw_Init(&rect);
        AgPainter_GetLocalDrawAera(painter, &rect.rect);
        rect.color = AG_COLOR_WHITE;
        painter->call_draw(painter, &rect.draw);
    }
    {
        AgFillDraw draw;
        AgFillDraw_Init(&draw);
        AgPainter_GetLocalDrawAera(painter, &draw.rect);
        AgRect_Expand(&draw.rect, -1, -1);
        draw.rect.x = x;
        draw.rect.w = x_end - x;
        draw.color = AG_COLOR_WHITE;
        painter->call_draw(painter, &draw.draw);
    }
}

static void Lv_ScrollBarV(AgPainter* painter, ag_int16 y, ag_int16 y_end) {
    {
        AgFillDraw draw;
        AgFillDraw_Init(&draw);
        AgPainter_GetLocalDrawAera(painter, &draw.rect);
        draw.color = AG_COLOR_BLACK;
        painter->call_draw(painter, &draw.draw);
    }
    {
        AgRectDraw rect;
        AgRectDraw_Init(&rect);
        AgPainter_GetLocalDrawAera(painter, &rect.rect);
        rect.color = AG_COLOR_WHITE;
        painter->call_draw(painter, &rect.draw);
    }
    {
        AgFillDraw draw;
        AgFillDraw_Init(&draw);
        AgPainter_GetLocalDrawAera(painter, &draw.rect);
        AgRect_Expand(&draw.rect, -1, -1);
        draw.rect.y = y;
        draw.rect.h = y_end - y;
        draw.color = AG_COLOR_WHITE;
        painter->call_draw(painter, &draw.draw);
    }
}


static ag_int16 Count(AgListModel* model) {
    return 100;
}

static void DrawItem(AgPainter* painter, ag_int16 idx, ag_bool selected) {
    {
        AgFillDraw draw;
        AgFillDraw_Init(&draw);
        AgPainter_GetLocalDrawAera(painter, &draw.rect);
        AgRect_Expand(&draw.rect, -1, -1);
        draw.color = selected == ag_true ? AG_COLOR_WHITE : AG_COLOR_BLACK;
        painter->call_draw(painter, &draw.draw);
    }
    {
        char buff[32];
        sprintf(buff, "%d", idx);
        AgTextDraw draw;
        AgTextDraw_Init(&draw);
        draw.align = eAgAlign_Center;
        draw.color = selected == ag_true ? AG_COLOR_BLACK : AG_COLOR_WHITE;
        draw.text = buff;
        draw.font_size = 20;
        AgPainter_GetLocalDrawAera(painter, &draw.rect);
        painter->call_draw(painter, &draw.draw);
    }
}

static void Event(AgListModel* model, AgListView* lv, AgEvent* event, ag_int16 idx) {
    /* nothing */
}

static ag_int16 Height(AgListView* lv, ag_int16 idx) {
    return lv->style == eAgListViewStyle_Horizontal ? 50 : 30;
}

int main() {
    AgRect screen = {0, 0, 800, 600};
    InitWindow(800, 600, "raylib [examples] example - list view");
    SetTargetFPS(60);

    PainterImpl impl;
    PainterImpl_Init(&impl);

    /* obj */
    AgObj root;
    AgObj_Init(&root);
    AgObj_SetBound(&root, &screen);

    AgListModel model;
    model.count = Count;
    model.draw = DrawItem;
    model.event = Event;
    model.height = Height;

    AgListView horizontal;
    AgListView_Init(&horizontal, &root, NULL);
    AgObj_SetBounds(&horizontal.obj, 0, 0, screen.w, 100);
    AgListView_SetStyle(&horizontal, eAgListViewStyle_Horizontal);
    horizontal.background = Lv_BackGround;
    horizontal.scroll_bar = Lv_ScrollBarH;
    horizontal.scroll_bar_width = 20;
    AgListView_SetModel(&horizontal, &model);
    AgListView_SetSelectIdx(&horizontal, 5);

    AgListView vectical;
    AgListView_Init(&vectical, &root, NULL);
    AgObj_SetBounds(&vectical.obj, 0, 100, screen.w ,screen.h - 100);
    vectical.background = Lv_BackGround;
    vectical.scroll_bar = Lv_ScrollBarV;
    vectical.scroll_bar_width = 20;
    AgListView_SetModel(&vectical, &model);
    AgListView_SetBeginIdx(&vectical, 5);

    /* loop */
    while (!WindowShouldClose()) {
        if (root.flags.invalid) {
            AgObj_DrawObj(&root, &impl.painter);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        Vector2 pos = {.x = 0, .y = 0};
        Rectangle rect = {0 ,0, screen.w, -screen.h};
        DrawTextureRec(impl.texture.texture, rect, pos, WHITE);
        EndDrawing();
    }
    CloseWindow();
}