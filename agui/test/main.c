#include "ag_key/ag_key.h"
#include "ag_comp/button.h"
#include "ag_comp/text_view.h"



// ---------------------------------------- clear ----------------------------------------
static void ClearDraw(AgObj* obj, AgPainter* painter) {
    FillDraw fill = {
        .color = AG_COLOR_BLACK,
    };
    FillDraw_Init(&fill, painter);
    AgObj_GetLocalBound(obj, &fill.rect);
    painter->call_draw(painter, &fill.draw);
}

// ---------------------------------------- main ----------------------------------------
int main(int argc, char** argv) {
    InitWindow(800, 450, "raylib [core] example - basic window");
    SetTargetFPS(60);

    /* painter */
    PainterImpl impl;
    PainterImpl_Init(&impl);

    /* root */
    AgObj root;
    AgObj_Init(&root);
    AgRect rect = {
        .x = 0,
        .y = 0,
        .w = 800,
        .h = 450
    };
    AgObj_SetBound(&root, &rect);
    Root_Layout(&root);
    root.vfunc.draw = ClearDraw;

    AgObj_CalcBound(&root);

    while (!WindowShouldClose()) {
        if (root.flags.invalid) {
            AgObj_DrawObj(&root, &impl.painter);
        }
    }

    CloseWindow();
    return 0;
}