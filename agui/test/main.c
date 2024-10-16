#include "ag_key/ag_key.h"
#include "ag_comp/button.h"
#include "ag_comp/text_view.h"
#include "agui/layout/ag_im_layout.h"
#include "ag_comp/ag_stack_layout.h"

// ---------------------------------------- widget ----------------------------------------
static AgObj page1;
static AgTextView tv1 = {
    .align = eAgAlign_YTop | eAgAlign_XCenter,
    .color = {255, 255, 255, 255},
    .font_size = 32,
    .text = "page1"
};
static AgButton btn1 = {
    .color = {255, 255, 255, 255},
    .font_size = 16,
    .text = "jump to page2"
};

static AgObj page2;
static AgTextView tv2 = {
    .align = eAgAlign_YTop | eAgAlign_XCenter,
    .color = {255, 255, 255, 255},
    .font_size = 32,
    .text = "page2"
};
static AgButton btn2 = {
    .color = {255, 255, 255, 255},
    .font_size = 16,
    .text = "jump to page1"
};

static AgStackLayout sl;

static void Page1_Layout(AgObj* obj) {
    AgImLayout_Layout(&tv1.obj, eAgAlign_Center);
    AgImLayout_Layout(&btn1.obj, eAgAlign_XCenter | eAgAlign_YBottom);
}

static void Page2_Layout(AgObj* obj) {
    AgImLayout_Layout(&tv2.obj, eAgAlign_Center);
    AgImLayout_Layout(&btn2.obj, eAgAlign_XCenter | eAgAlign_YBottom);
}

static void Root_Layout(AgObj* obj) {
    AgRect parent;
    AgObj_GetLocalBound(obj, &parent);
    AgObj_SetBound(&sl.obj, &parent);
}

static void Btn1_Event(AgObj* obj, const AgEvent* event) {
    if (event->type == 1) { // push
        AgButton_SetPress(&btn1, ag_true);
    }
    else {
        AgButton_SetPress(&btn1, ag_false);
        AgStackLayout_Push2(&sl, &page2);
    }
}

static void Btn2_Event(AgObj* obj, const AgEvent* event) {
    if (event->type == 1) { // push
        AgButton_SetPress(&btn2, ag_true);
    }
    else {
        AgButton_SetPress(&btn2, ag_false);
        // AgStackLayout_Pop(&sl);
        AgStackLayout_Push2(&sl, &page1);
    }
}

// ---------------------------------------- layout ----------------------------------------
static void Root_Init(AgObj* obj) {
    AgObj_Init(&page1);
    AgTextView_Init(&tv1);
    AgButton_Init(&btn1);
    AgObj_AddChild(&page1, &tv1.obj);
    AgObj_AddChild(&page1, &btn1.obj);
    AgObj_SetSize(&btn1.obj, 200, 50);
    page1.vfunc.layout = Page1_Layout;
    btn1.obj.vfunc.event = Btn1_Event;

    AgObj_Init(&page2);
    AgTextView_Init(&tv2);
    AgButton_Init(&btn2);
    AgObj_AddChild(&page2, &tv2.obj);
    AgObj_AddChild(&page2, &btn2.obj);
    AgObj_SetSize(&btn2.obj, 200, 50);
    page2.vfunc.layout = Page2_Layout;
    btn2.obj.vfunc.event = Btn2_Event;

    AgStackLayout_Init(&sl);
    AgStackLayout_Push(&sl, &page1);
    AgObj_AddChild(obj, &sl.obj);
    obj->vfunc.layout = Root_Layout;
}

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
    Root_Init(&root);
    AgObj_SetBound(&root, &rect);
    root.vfunc.draw = ClearDraw;

    while (!WindowShouldClose()) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 pos = GetMousePosition();
            AgObj* obj = AgObj_HitObj(&root, pos.x, pos.y);
            if (NULL != obj) {
                AgEvent e = {
                    .type = 1
                };
                AgObj_SendEvent(obj, &e);
            }
        }
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            Vector2 pos = GetMousePosition();
            AgObj* obj = AgObj_HitObj(&root, pos.x, pos.y);
            if (NULL != obj) {
                AgEvent e = {
                    .type = 2
                };
                AgObj_SendEvent(obj, &e);
            }
        }

        AgObj_Redraw(&root);
        if (root.flags.invalid) {
            AgObj_DrawObj(&root, &impl.painter);
        }
    }

    CloseWindow();
    return 0;
}