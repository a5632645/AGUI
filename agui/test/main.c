#include "raylib.h"
#include "ag_impl/painter_impl.h"
#include "ag_impl/draws_impl.h"
#include "ag_comp/ag_list_view.h"
#include "stdio.h"
#include "stdlib.h"

// ---------------------------------------- model ----------------------------------------
static ag_int16 Count() {
    return 100;
}

static void Draw(AgPainter* painter, ag_int16 idx, ag_bool selected) {
    {
        AgFillDraw draw;
        AgFillDraw_Init(&draw, painter);
        draw.color = selected ? AG_COLOR_GRAY : AG_COLOR_BLACK;

        AgRect_Copy(&draw.rect, &painter->draw_aera);
        draw.rect.x = 0;
        draw.rect.y = 0;

        painter->call_draw(painter, &draw.draw);
    }

    AgTextDraw draw;
    AgTextDraw_Init(&draw, painter);
    draw.align = eAgAlign_Center;
    draw.color = AG_COLOR_WHITE;
    draw.font_size = 20;

    AgRect_Copy(&draw.rect, &painter->draw_aera);
    draw.rect.x = 0;
    draw.rect.y = 0;

    char buff[20] = {};
    draw.text = buff;
    sprintf(buff, "%d", idx);
    painter->call_draw(painter, &draw.draw);
}

static void Event(AgListView* lv, AgEvent* event, ag_int16 idx) {
    
}

static ag_int16 Height(ag_int16 idx) {
    return 31;
}

// ---------------------------------------- model2 ----------------------------------------
static ag_int16 model2_count = 0;
static ag_int16 Count2() {
    return model2_count;
}

static ag_int16 Count3() {
    return 10;
}

static ag_int16 Count4() {
    return 200;
}

static ag_int16 Height2(ag_int16 idx) {
    return idx % 40 + 1;
}

static void DrawScroll(AgPainter* painter, ag_int16 y, ag_int16 y_end) {
    {
        AgFillDraw draw;
        AgFillDraw_Init(&draw, painter);
        AgPainter_GetLocalDrawAera(painter, &draw.rect);
        draw.color = AG_COLOR_BLACK;
        painter->call_draw(painter, &draw.draw);
    }
    {
        AgRectDraw draw;
        AgRectDraw_Init(&draw, painter);
        AgPainter_GetLocalDrawAera(painter, &draw.rect);
        draw.color = AG_COLOR_GRAY;
        painter->call_draw(painter, &draw.draw);
    }
    {
        AgLineDraw draw;
        AgLineDraw_Init(&draw, painter);
        draw.color = AG_COLOR_WHITE;
        draw.x1 = painter->draw_aera.w / 2;
        draw.y1 = 0;
        draw.y2 = painter->draw_aera.h;
        draw.x2 = draw.x1;
        painter->call_draw(painter, &draw.draw);
    }
    {
        AgRectDraw draw;
        AgRectDraw_Init(&draw, painter);
        draw.rect.x = 0;
        draw.rect.y = y;
        draw.rect.w = painter->draw_aera.w;
        draw.rect.h = y_end - y;
        draw.color = AG_COLOR_WHITE;
        painter->call_draw(painter, &draw.draw);
    }
}

static void LvBackground(AgPainter* painter, const AgRect* rect) {
    AgFillDraw draw;
    AgFillDraw_Init(&draw, painter);
    draw.color = AG_COLOR_BLACK;
    AgRect_Copy(&draw.rect, rect);
    painter->call_draw(painter, &draw.draw);
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
    lv.background = LvBackground;
    lv.scroll_bar = DrawScroll;
    lv.scroll_bar_width = 16;

    AgListModel model;
    model.count = Count;
    model.draw = Draw;
    model.event = Event;
    model.height = Height;
    AgListView_SetModel(&lv, &model);

    AgListModel model2;
    model2.count = Count2;
    model2.draw = Draw;
    model2.event = Event;
    model2.height = Height;

    AgListModel model3;
    model3.count = Count3;
    model3.draw = Draw;
    model3.event = Event;
    model3.height = Height2;

    AgListModel model4;
    model4.count = Count4;
    model4.draw = Draw;
    model4.event = Event;
    model4.height = Height2;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_UP)) {
            /* 切换测试 */
            AgListView_Scroll(&lv, -1);
        }
        if (IsKeyPressed(KEY_DOWN)) {
            /* 切换测试 */
            AgListView_Scroll(&lv, 1);
        }
        if (IsKeyPressed(KEY_A)) {
            /* 更新测试 */
            static int testpos = 0;
            if (testpos == 0) AgListView_ItemChange(&lv, -1);
            else if (testpos == 1) AgListView_ItemChange(&lv, 0);
            else if (testpos == 2) AgListView_ItemChange(&lv, 1);
            else if (testpos == 3) AgListView_ItemChange(&lv, 2);
            else if (testpos == 4) AgListView_ItemChange(&lv, 3);
            else if (testpos == 5) AgListView_ItemChange(&lv, 4);
            else if (testpos == 6) AgListView_ItemChange(&lv, 5);
            else if (testpos == 7) AgListView_ItemChange(&lv, 6);
            else if (testpos == 8) AgListView_ItemChange(&lv, 7);
            else if (testpos == 9) AgListView_ItemChange(&lv, 8);
            else if (testpos == 10) AgListView_ItemChange(&lv, 9);
            else if (testpos == 11) AgListView_ItemChange(&lv, 10);
            else if (testpos == 12) AgListView_ItemChange(&lv, 110);
            else if (testpos == 13) AgListView_ItemChange(&lv, 20);
            ++testpos;
            testpos %= 13;
        }
        if (IsKeyPressed(KEY_B)) {
            /* 滑动测试 */
            static int testpos = 0;
            if (testpos == 0) AgListView_Scroll(&lv, -100);
            else if (testpos == 1) AgListView_Scroll(&lv, -10);
            else if (testpos == 2) AgListView_Scroll(&lv, -1);
            else if (testpos == 3) AgListView_Scroll(&lv, 1);
            else if (testpos == 4) AgListView_Scroll(&lv, 10);
            else if (testpos == 5) AgListView_Scroll(&lv, 100);
            ++testpos;
            testpos %= 6;
        }
        if (IsKeyPressed(KEY_C)) {
            /* 改变起始点测试 */
            static int testpos = 0;
            if (testpos == 0) AgListView_SetBeginIdx(&lv, -100);
            else if (testpos == 1) AgListView_SetBeginIdx(&lv, -10);
            else if (testpos == 2) AgListView_SetBeginIdx(&lv, -1);
            else if (testpos == 3) AgListView_SetBeginIdx(&lv, 0);
            else if (testpos == 4) AgListView_SetBeginIdx(&lv, 1);
            else if (testpos == 5) AgListView_SetBeginIdx(&lv, 10);
            else if (testpos == 6) AgListView_SetBeginIdx(&lv, 100);
            else if (testpos == 6) AgListView_SetBeginIdx(&lv, 200);
            ++testpos;
            testpos %= 7;
        }
        if (IsKeyPressed(KEY_D)) {
            /* 模型测试 */
            static int testpos = 0;
            if (testpos == 0) AgListView_SetModel(&lv, NULL);
            else if (testpos == 1) AgListView_SetModel(&lv, &model2);
            else if (testpos == 2) AgListView_SetModel(&lv, &model);
            else if (testpos == 3) AgListView_SetModel(&lv, &model3);
            else if (testpos == 4) AgListView_SetModel(&lv, &model4);
            ++testpos;
            testpos %= 5;
        }
        if (IsKeyPressed(KEY_E)) {
            /* 更新测试 */
            AgListView_Update(&lv);
        }
        if (IsKeyPressed(KEY_F)) {
            /* 动态修改大小测试 */
            model2_count = rand() % 21;
            model2_count = AGUI_MAX(0, model2_count);
            AgListView_Update(&lv);
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