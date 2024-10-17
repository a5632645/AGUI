#include "raylib.h"
#include "ag_impl/painter_impl.h"
#include "ag_impl/draws_impl.h"
#include "agui/ag_obj.h"
#include "ag_comp/button.h"
#include "ag_comp/ag_stack_layout.h"
#include "ag_comp/text_view.h"
#include "agui/ag_types.h"
#include "agui/layout/ag_im_layout.h"
#include "ag_key/ag_key.h"

static AgObj root;
    static AgObj buttons;
        static AgButton btn1;
        static AgButton btn2;
        static AgButton btn3;
        static AgButton btn4;
    static AgStackLayout sl;
        static AgObj obj1;
            static AgTextView tv1;
            static AgButton btn11;
        static AgObj obj2;
            static AgTextView tv2;
            static AgButton btn21;
        static AgObj obj3;
            static AgTextView tv3;
            static AgButton btn31;
        static AgObj obj4;
            static AgTextView tv4;
            static AgButton btn41;
static AgKeySwitcher switcher;
static AgObj highlighter;

// ---------------------------------------- funcs ----------------------------------------
static void Page_Layout(AgObj* obj) {
    AgObj* tv = AGUI_CONTAINER_OF(AgObj, node, obj->childern.head);
    AgObj* btn = AGUI_CONTAINER_OF(AgObj, node, obj->childern.head->next);
    AgObj_SetSize(btn, 100, 40);
    AgImLayout_Layout(tv, eAgAlign_Center);
    AgImLayout_Layout(btn, eAgAlign_XCenter | eAgAlign_YBottom);
}

static void Root_Layout(AgObj* obj) {
    ag_int16 w = obj->bound.w;
    ag_int16 h = obj->bound.h;
    AgObj_SetBounds(&buttons, 0, 0, 100, h);
    AgObj_SetBounds(&sl.obj, 100, 0, w - 100, h);
}

static void Hightlight_Draw(AgObj* obj, AgPainter* painter) {
    RectDraw draw = {
        .color = AG_COLOR_GRAY
    };
    RectDraw_Init(&draw, painter);
    AgObj_GetLocalBound(obj, &draw.rect);
    painter->call_draw(painter, &draw.draw);
}

// ---------------------------------------- filters ----------------------------------------
static ag_bool Buttons_Filter(AgObj* obj) {
    return true;
}

static ag_bool Buttons_ActionFilter(AgObj* curr, AgKeySwitActionEnum action) {
    switch (action) {
    case eAgKeyAction_GoNext:
    case eAgKeyAction_GoPrev:
        return ag_true;
    default:
        return ag_false;
    }
}

static void Buttons_ObjSelected(AgObj* obj) {
    if (obj == &btn1.obj) {
        AgStackLayout_Push2(&sl, &obj1);
    }
    else if (obj == &btn2.obj) {
        AgStackLayout_Push2(&sl, &obj2);
    }
    else if (obj == &btn3.obj) {
        AgStackLayout_Push2(&sl, &obj3);
    }
    else if (obj == &btn4.obj) {
        AgStackLayout_Push2(&sl, &obj4);
    }
}

// ---------------------------------------- layout ----------------------------------------
static void Root_Init() {
    AgObj_Init(&root);
        AgObj_Init(&buttons);
            AgButton_Init(&btn1);
            btn1.text = "page1";
            AgObj_AddChild(&buttons, &btn1.obj);
            AgObj_SetBounds(&btn1.obj, 0, 0, 100, 40);
            AgButton_Init(&btn2);
            btn2.text = "page2";
            AgObj_AddChild(&buttons, &btn2.obj);
            AgObj_SetBounds(&btn2.obj, 0, 50, 100, 40);
            AgButton_Init(&btn3);
            btn3.text = "page3";
            AgObj_AddChild(&buttons, &btn3.obj);
            AgObj_SetBounds(&btn3.obj, 0, 100, 100, 40);
            AgButton_Init(&btn4);
            btn4.text = "page4";
            AgObj_AddChild(&buttons, &btn4.obj);
            AgObj_SetBounds(&btn4.obj, 0, 150, 100, 40);
        AgObj_AddChild(&root, &buttons);
        AgStackLayout_Init(&sl);
            AgObj_Init(&obj1);
                AgTextView_Init(&tv1);
                tv1.text = "page1";
                AgObj_AddChild(&obj1, &tv1.obj);
                AgButton_Init(&btn11);
                btn11.text = "jmp page2";
                obj1.vfunc.layout = Page_Layout;
                AgObj_AddChild(&obj1, &btn11.obj);
            AgObj_Init(&obj2);
                AgTextView_Init(&tv2);
                tv2.text = "page2";
                AgObj_AddChild(&obj2, &tv2.obj);
                AgButton_Init(&btn21);
                btn21.text = "jmp page3";
                obj2.vfunc.layout = Page_Layout;
                AgObj_AddChild(&obj2, &btn21.obj);
            AgObj_Init(&obj3);
                AgTextView_Init(&tv3);
                tv3.text = "page3";
                AgObj_AddChild(&obj3, &tv3.obj);
                AgButton_Init(&btn31);
                btn31.text = "jmp page4";
                obj3.vfunc.layout = Page_Layout;
                AgObj_AddChild(&obj3, &btn31.obj);
            AgObj_Init(&obj4);
                AgTextView_Init(&tv4);
                tv4.text = "page4";
                AgObj_AddChild(&obj4, &tv4.obj);
                AgButton_Init(&btn41);
                btn41.text = "jmp page1";
                obj4.vfunc.layout = Page_Layout;
                AgObj_AddChild(&obj4, &btn41.obj);
        AgObj_AddChild(&root, &sl.obj);
    root.vfunc.layout = Root_Layout;

    AgObj_Init(&highlighter);
    highlighter.vfunc.draw = Hightlight_Draw;
    AgKeySwitcher_Init(&switcher, &root, &highlighter);
    switcher.action_filter = Buttons_ActionFilter;
    switcher.filter = Buttons_Filter;
    switcher.obj_selected = Buttons_ObjSelected;
    AgKeySwitcher_SetCurrent(&switcher, &btn1.obj);
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
    AgRect rect = {
        .x = 0,
        .y = 0,
        .w = 800,
        .h = 450
    };
    Root_Init();
    AgObj_SetBound(&root, &rect);
    root.vfunc.draw = ClearDraw;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_RIGHT)) {
            AgKeySwitcher_GoNext(&switcher);
        }
        if (IsKeyPressed(KEY_LEFT)) {
            AgKeySwitcher_GoPrev(&switcher);
        }
        if (IsKeyPressed(KEY_UP)) {
            AgKeySwitcher_GoUp(&switcher);
        }
        if (IsKeyPressed(KEY_DOWN)) {
            AgKeySwitcher_GoDown(&switcher);
        }
        if (IsKeyPressed(KEY_ENTER)) {
            AgObj* obj = switcher.current;
            AgEvent e = {
                .handled = ag_false,
                .type = KEY_ENTER,
                .sender = obj,
            };
            if (NULL != obj) {
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