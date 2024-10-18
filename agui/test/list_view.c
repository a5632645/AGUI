#include "raylib.h"
#include "ag_impl/painter_impl.h"
#include "ag_comp/ag_list_view.h"
#include "stdio.h"
#include "stdlib.h"

static ag_uint32 Count() {
    return 100;
}

static void Draw(const AgObj* obj, AgPainter* painter, ag_uint32 idx, ag_bool selected) {
    {
        AgFillDraw draw;
        AgFillDraw_Init(&draw, painter);
        draw.color = selected ? AG_COLOR_GRAY : AG_COLOR_BLACK;
        AgObj_GetLocalBound(obj, &draw.rect);
        painter->call_draw(painter, &draw.draw);
    }

    AgTextDraw draw;
    AgTextDraw_Init(&draw, painter);
    draw.align = eAgAlign_Center;
    draw.color = AG_COLOR_WHITE;
    draw.font_size = 20;
    AgObj_GetLocalBound(obj, &draw.rect);
    char buff[20] = {};
    draw.text = buff;
    sprintf(buff, "%d", idx);
    painter->call_draw(painter, &draw.draw);
}

static void Event(AgListView* lv, AgEvent* event, ag_uint32 idx) {
    
}

static ag_int16 Height(ag_uint32 idx) {
    return 31;
}

int main(int argc, char** argv) {
    AgRect screen = {0, 0, 800, 600};
    InitWindow(screen.w, screen.h, "raylib [core] example - basic window");
    SetTargetFPS(60);

    PainterImpl impl;
    PainterImpl_Init(&impl);

    AgListView lv;
    AgListView_Init(&lv, NULL, NULL);
    AgObj_SetBound(&lv.obj, &screen);

    AgListModel model;
    model.count = Count;
    model.draw = Draw;
    model.event = Event;
    model.height = Height;
    AgListView_SetModel(&lv, &model);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_UP)) {
            AgListView_Scroll(&lv, -1);
        }
        if (IsKeyPressed(KEY_DOWN)) {
            AgListView_Scroll(&lv, 1);
        }

        if (ag_true == lv.obj.flags.invalid) {
            AgObj_DrawObj(&lv.obj, &impl.painter);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        Rectangle rect = {0, 0, screen.w, -screen.h};
        Vector2 pos = {0, 0};
        DrawTextureRec(impl.texture.texture, rect, pos, WHITE);
        EndDrawing();
    }
}