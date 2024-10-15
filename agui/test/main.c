#include "ag_key/ag_key.h"
#include "ag_comp/button.h"
#include "ag_comp/text_view.h"

// ---------------------------------------- highlighter ----------------------------------------
typedef struct {
    AgObj obj;
} Highlighter;

static void Highlighter_Draw(AgObj* obj, AgPainter* painter) {
    Highlighter* h = AGUI_CONTAINER_OF(Highlighter, obj, obj);
    RectDraw fill = {
        .color = AG_COLOR_WHITE,
        .rect = obj->local_bound
    };
    RectDraw_Init(&fill, painter);
    painter->call_draw(painter, &fill.draw);
}

static void _Highlighter_Init(Highlighter* h) {
    AgObj_Init(&h->obj);
    h->obj.vfunc.draw = Highlighter_Draw;
}

// ---------------------------------------- layout ----------------------------------------
static void Tree1_Layout(AgObj* obj) {
    static AgTextView tv1 = {
        .align = eAgAlign_None,
        .color = {255, 255, 255, 255},
        .font_size = 16,
        .text = "Tree1 Obj1"
    };
    AgTextView_Init(&tv1);
    AgObj_SetSize(&tv1.obj, 100, 30);
    AgObj_SetPos(&tv1.obj, 0 , 0);

    static AgTextView tv2 = {
        .align = eAgAlign_None,
        .color = {255, 255, 255, 255},
        .font_size = 16,
        .text = "Tree1 Obj2"
    };
    AgTextView_Init(&tv2);
    AgObj_SetSize(&tv2.obj, 100, 30);
    AgObj_SetPos(&tv2.obj, 100 , 0);

    static AgTextView tv3 = {
        .align = eAgAlign_None,
        .color = {255, 255, 255, 255},
        .font_size = 16,
        .text = "Tree1 Obj3"
    };
    AgTextView_Init(&tv3);
    AgObj_SetSize(&tv3.obj, 100, 30);
    AgObj_SetPos(&tv3.obj, 200 , 0);

    AgObj_AddChild(obj, &tv1.obj);
    AgObj_AddChild(obj, &tv2.obj);
    AgObj_AddChild(obj, &tv3.obj);
}

static void Tree2_Layout(AgObj* obj) {
    static AgTextView tv1 = {
        .align = eAgAlign_None,
        .color = {255, 255, 255, 255},
        .font_size = 16,
        .text = "Tree2 Obj1"
    };
    AgTextView_Init(&tv1);
    AgObj_SetSize(&tv1.obj, 100, 30);
    AgObj_SetPos(&tv1.obj, 0 , 0);

    static AgTextView tv2 = {
        .align = eAgAlign_None,
        .color = {255, 255, 255, 255},
        .font_size = 16,
        .text = "Tree2 Obj2"
    };
    AgTextView_Init(&tv2);
    AgObj_SetSize(&tv2.obj, 100, 30);
    AgObj_SetPos(&tv2.obj, 100 , 0);

    static AgTextView tv3 = {
        .align = eAgAlign_None,
        .color = {255, 255, 255, 255},
        .font_size = 16,
        .text = "Tree2 Obj3"
    };
    AgTextView_Init(&tv3);
    AgObj_SetSize(&tv3.obj, 100, 30);
    AgObj_SetPos(&tv3.obj,200 , 0);

    AgObj_AddChild(obj, &tv1.obj);
    AgObj_AddChild(obj, &tv2.obj);
    AgObj_AddChild(obj, &tv3.obj);
}

static void CustomBtnEvent(AgObj* obj, const AgEvent* event) {
    AgButton* btn = AGUI_CONTAINER_OF(AgButton, obj, obj);
    btn->press = !btn->press;
    AgObj_MarkRedraw(&btn->obj);

    AgTextView* tv = AGUI_CONTAINER_OF(AgTextView, obj, obj->node.prev);
    tv->text = btn->press ? "Press" : "Release";
    AgObj_MarkRedraw(&tv->obj);

    AgKeySwitcher* ks = (AgKeySwitcher*)event->data;
    AgKeySwitcher_SetCurrent(ks, AGUI_CONTAINER_OF(AgObj, node, ks->root->childern.head));
}

static void Tree3_Layout(AgObj* obj) {
    static AgTextView tv1 = {
        .align = eAgAlign_Center,
        .color = {255, 255, 255, 255},
        .font_size = 16,
        .text = "Triger TextView"
    };
    AgTextView_Init(&tv1);
    AgObj_SetSize(&tv1.obj, 300, 30);
    AgObj_SetPos(&tv1.obj, 0 , 0);

    static AgButton btn1 = {
        .color = {255, 255, 255, 255},
        .font_size = 16,
        .text = "Triger Button"
    };
    AgButton_Init(&btn1);
    AgObj_SetSize(&btn1.obj, 300, 100);
    AgObj_SetPos(&btn1.obj, 0 , 30);
    btn1.obj.vfunc.event = CustomBtnEvent;

    AgObj_AddChild(obj, &tv1.obj);
    AgObj_AddChild(obj, &btn1.obj);
}

static void Root_Layout(AgObj* obj) {
    static AgObj tree1;
    AgObj_Init(&tree1);
    AgObj_SetPos(&tree1, 0, 0);
    Tree1_Layout(&tree1);

    static AgObj tree2;
    AgObj_Init(&tree2);
    AgObj_SetPos(&tree2, 0, 50);
    Tree2_Layout(&tree2);

    static AgObj tree3;
    AgObj_Init(&tree3);
    AgObj_SetPos(&tree3, 0, 100);
    Tree3_Layout(&tree3);

    AgObj_AddChild(obj, &tree1);
    AgObj_AddChild(obj, &tree2);
    AgObj_AddChild(obj, &tree3);
}

// ---------------------------------------- clear ----------------------------------------
static void ClearDraw(AgObj* obj, AgPainter* painter) {
    FillDraw fill = {
        .color = AG_COLOR_BLACK,
        .rect = obj->local_bound
    };
    FillDraw_Init(&fill, painter);
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

    /* highlighter */
    Highlighter h;
    _Highlighter_Init(&h);

    /* keyswitcher */
    AgKeySwitcher ks;
    AgKeySwitcher_Init(&ks, &root, &h.obj);
    AgObj_SetBound(&h.obj, &ks.current->bound);

    AgObj_CalcBound(&root);

    while (!WindowShouldClose()) {
        // BeginDrawing();
        
        /* key */
        if (IsKeyPressed(KEY_UP)) {
            AgKeySwitcher_GoUp(&ks);
        }
        if (IsKeyPressed(KEY_DOWN)) {
            AgKeySwitcher_GoDown(&ks);
        }
        if (IsKeyPressed(KEY_LEFT)) {
            AgKeySwitcher_GoPrev(&ks);
        }
        if (IsKeyPressed(KEY_RIGHT)) {
            AgKeySwitcher_GoNext(&ks);
        }
        if (IsKeyPressed(KEY_ENTER)) {
            AgEvent e = {
                .type = 1,
                .data = &ks
            };
            AgObj_SendEvent(ks.current, &e);
        }

        /* render */
        AgObj_MarkRedraw(&root);
        AgObj_DrawObj(&root, &impl.painter);

        // EndDrawing();
    }

    CloseWindow();
    return 0;
}