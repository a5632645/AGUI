#include "agui/port/painter_impl.h"
#include "text_view.h"
#include "agui/port/draws_impl.h"

// ---------------------------------------- declare ----------------------------------------
static void CustomDraw(AgObj* obj, AgPainter* painter) {
    AgObj_MarkRedraw(obj);
    static int x = 0;
    AgObj* child0 = AGUI_CONTAINER_OF(AgObj, node, obj->childern.head);
    AgObj_SetPos(child0, x, child0->bound.y);
    ++x;
    x %= 300;
    if (x > 200) {
        AgObj_SetVisiable(obj, ag_false);
    }
}

static void ClearDraw(AgObj* obj, AgPainter* painter) {
    FillDraw draw = {
        .rect = obj->bound
    };
    FillDraw_Init(&draw, painter);
    painter->set_color(painter, AG_COLOR_BLACK);
    painter->call_draw(painter, &draw.draw);
}

// ---------------------------------------- main ----------------------------------------
int main(int argc, char** argv) {
    InitWindow(800, 450, "raylib [core] example - basic window");
    SetTargetFPS(60);


    PainterImpl impl;
    PainterImpl_Init(&impl);

    // tree 1
    TextView view = {
        .font_size = 16,
        .text = "Hello, World!",
        .color = AG_COLOR_WHITE
    };
    TextView_Init(&view);
    AgObj_SetSize(&view.obj, 100, 50);
    TextView tv2 = {
        .font_size = 16,
        .text = "Hello, World2!",
        .color = AG_COLOR_WHITE
    };
    TextView_Init(&tv2);

    // tree 2
    AgObj tree2;
    AgObj_Init(&tree2);
    tree2.vfunc.draw = CustomDraw;
    TextView tv3 = {
        .font_size = 16,
        .text = "Hello, World3!",
        .color = AG_COLOR_WHITE
    };
    TextView_Init(&tv3);
    AgObj_AddChild(&tree2, &tv3.obj);

    // root
    AgObj root;
    AgObj_Init(&root);
    root.vfunc.draw = ClearDraw;
    AgObj_AddChild(&root, &view.obj);
    AgObj_AddChild(&root, &tv2.obj);
    AgObj_AddChild(&root, &tree2);
    AgRect rect = { 0, 0, 800, 450 };
    AgObj_SetBound(&root, &rect);

    // 布局调整
    AgObj_SetPos(&tree2, 0, 100);
    AgObj_SetPos(&tv3.obj, 20, 0);
    AgObj_SetPos(&tv2.obj, 0, 50);

    while (!WindowShouldClose()) {
        // BeginDrawing();
        
        if (root.flags.invalid) {
            AgObj_DrawObj(&root, &impl.painter);
        }

        // EndDrawing();
    }
    CloseWindow();
    return 0;
}