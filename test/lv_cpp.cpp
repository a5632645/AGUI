#include "painter.hpp"
#include "ag_comp/ag_list_view.hpp"
#include <iostream>

class TModel : public agui::AgListModel {
public:
    int16_t Count() override {
        return 100;
    }

    int16_t Length(agui::AgListViewStyle style, int16_t idx) override {
        return 8;
    }

    void Draw(agui::AgPainter& painter, int16_t idx, agui::ag_bool selected) override {
        FillDraw draw;
        draw.rect = painter.GetLocalDrawAera();
        draw.color = selected ? agui::colors::kWhite : agui::colors::kBlack;
        painter.CallDraw(draw);
    }

    void Event(agui::AgListView& lv, agui::AgEvent& event, int16_t idx) override {

    }
};

class TDelegate : public agui::AgListViewDelegate {
public:
    int16_t ScrollBarWidth(agui::AgListViewStyle style) override { return 8; }

    void ScrollBar(agui::AgPainter& painter, agui::AgListViewStyle style, int16_t pixel_start, int16_t pixel_end) override {
        {
            FillDraw draw;
            draw.rect = painter.GetLocalDrawAera();
            draw.color = agui::colors::kBlack;
            painter.CallDraw(draw);
        }
        {
            RectDraw draw;
            draw.rect = painter.GetLocalDrawAera();
            draw.color = agui::colors::kWhite;
            painter.CallDraw(draw);
        }
        {
            FillDraw draw;
            draw.rect = painter.GetLocalDrawAera();
            draw.rect.y = pixel_start;
            draw.rect.h = pixel_end - pixel_start;
            draw.color = agui::colors::kWhite;
            painter.CallDraw(draw);
        }
    }

    void Background(agui::AgPainter& painter, const agui::AgRect& draw_aera) override {
        FillDraw draw;
        draw.rect = draw_aera;
        draw.color = agui::colors::kBlack;
        painter.CallDraw(draw);
    }
};

int main() {
    agui::AgRect screen = {0, 0, 800, 600};
    InitWindow(800, 600, "raylib [examples] example - list view");
    SetTargetFPS(60);

    agui::AgListView lv;
    TModel model;
    TDelegate delegate;
    TestPainter painter;
    lv.SetModel(&model);
    lv.SetDelegate(&delegate);
    lv.SetBound(screen);
    lv.SetStyle(agui::AgListViewStyle::eAgListViewStyle_Horizontal);

    /* loop */
    while (!WindowShouldClose()) {
        if (lv.NeedDraw()) {
            lv.DrawObj(painter);
        }

        if (IsKeyPressed(KEY_UP)) {
            lv.Scroll(-1);
        }
        if (IsKeyPressed(KEY_DOWN)) {
            lv.Scroll(1);
            std::cout << lv.GetSelectedIdx() << std::endl;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        Vector2 pos = {.x = 0, .y = 0};
        Rectangle rect = {0 ,0, screen.w, -screen.h};
        DrawTextureRec(painter.GetTexture().texture, rect, pos, WHITE);
        EndDrawing();
    }
    CloseWindow();
}