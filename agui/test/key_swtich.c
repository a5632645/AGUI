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

// ---------------------------------------- event ----------------------------------------
enum {
    eEvent_Key = 0
};

typedef struct {
    AgEvent event;
    enum {
        eKey_None,
        eKey_Up,
        eKey_Down,
        eKey_Left,
        eKey_Right,
        eKey_Enter
    } key;
} KeyEvent;

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
    AgRectDraw draw = {
        .color = AG_COLOR_GRAY
    };
    AgRectDraw_Init(&draw, painter);
    // AgInvertDraw draw;
    // AgInvertDraw_Init(&draw, painter);
    AgObj_GetLocalBound(obj, &draw.rect);
    painter->call_draw(painter, &draw.draw);
}

// ---------------------------------------- logics ----------------------------------------
/* buttons页面 */
static ag_bool Buttons_Filter(AgObj* obj);
static void Buttons_ObjSelected(AgObj* obj);
static void Buttons_Event(AgKeySwitcher* ks, AgEvent* event);

/* page页面 */
static ag_bool Page_Filter(AgObj* obj);
static void Page_ObjSelected(AgObj* obj);
static void Page_Event(AgKeySwitcher* ks, AgEvent* event);

// ---------------------------------------- private ----------------------------------------
static ag_bool Buttons_Filter(AgObj* obj) {
    return true;
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

static void Buttons_Event(AgKeySwitcher* ks, AgEvent* event) {
    if (event->type == eEvent_Key) {
        event->handled = ag_true;
        
        KeyEvent* e = AGUI_CONTAINER_OF(KeyEvent, event, event);
        switch (e->key) {
        case eKey_Up:
            AgKeySwitcher_GoPrev(&switcher);
            break;
        case eKey_Down:
            AgKeySwitcher_GoNext(&switcher);
            break;
        case eKey_Right:
            /* 选择器到栈布局器的当前页面 */
            {
                AgObj* page = AgStackLayout_Current(&sl);
                if (NULL != page) {
                    AgObj* btn = AGUI_CONTAINER_OF(AgObj, node, page->childern.tail);
                    if (NULL != btn) {
                        /* 切换函数指针 */
                        ks->obj_selected = Page_ObjSelected;
                        ks->filter = Page_Filter;
                        ks->event = Page_Event;
                        /* 切换部件 */
                        AgKeySwitcher_Goto(&switcher, btn);
                    }
                }
            }
            break;
        default:
            break;
        }
    }
}

static ag_bool Page_Filter(AgObj* obj) {
    if (eAgObjType_TextView == obj->obj_type) {
        return ag_false;
    }
    return ag_true;
}
static void Page_ObjSelected(AgObj* obj) {

}
static void Page_Event(AgKeySwitcher* ks, AgEvent* event) {
    if (event->type == eEvent_Key) {
        event->handled = ag_true;
        KeyEvent* e = AGUI_CONTAINER_OF(KeyEvent, event, event);
        switch (e->key) {
        case eKey_Left:
        {
            /* 切换到按钮 */
            ks->event = Buttons_Event;
            ks->filter = Buttons_Filter;
            ks->obj_selected = Buttons_ObjSelected;
            AgKeySwitcher_Goto(&switcher, Agobj_FirstChild(&buttons));
        }
            break;
        case eKey_Enter:
        {
            AgObj* btn = switcher.current;
            if (&btn11.obj == btn) {
                AgStackLayout_Push2(&sl, &obj2);
                AgKeySwitcher_Goto(&switcher, AgObj_LastChild(&obj2));
            }
            else if (&btn21.obj == btn) {
                AgStackLayout_Push2(&sl, &obj3);
                AgKeySwitcher_Goto(&switcher, AgObj_LastChild(&obj3));
            }
            else if (&btn31.obj == btn) {
                AgStackLayout_Push2(&sl, &obj4);
                AgKeySwitcher_Goto(&switcher, AgObj_LastChild(&obj4));
            }
            else if (&btn41.obj == btn) {
                AgStackLayout_Push2(&sl, &obj1);
                AgKeySwitcher_Goto(&switcher, AgObj_LastChild(&obj1));
            }
        }
            break;
        default:
            break;
        }
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
    switcher.filter = Buttons_Filter;
    switcher.obj_selected = Buttons_ObjSelected;
    switcher.event = Buttons_Event;
    AgKeySwitcher_Goto(&switcher, &btn1.obj);
}

// ---------------------------------------- clear ----------------------------------------
static void ClearDraw(AgObj* obj, AgPainter* painter) {
    AgFillDraw fill = {
        .color = AG_COLOR_BLACK,
    };
    AgFillDraw_Init(&fill, painter);
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
        /* event */
        KeyEvent e = {
            .key = eKey_None,
            .event = {
                .handled = ag_false,
                .sender = NULL,
                .type = eEvent_Key
            }
        };
        if (IsKeyPressed(KEY_RIGHT)) {
            e.key = eKey_Right;
        }
        if (IsKeyPressed(KEY_LEFT)) {
            e.key = eKey_Left;
        }
        if (IsKeyPressed(KEY_UP)) {
            e.key = eKey_Up;
        }
        if (IsKeyPressed(KEY_DOWN)) {
            e.key = eKey_Down;
        }
        if (IsKeyPressed(KEY_ENTER)) {
            e.key = eKey_Enter;
        }
        if (e.key != eKey_None) {
            AgKeySwitcher_SendEvent(&switcher, &e.event);
        }

        /* draw */
        if (root.flags.invalid) {
            AgObj_DrawObj(&root, &impl.painter);
        }

        /* draw buffer */
        Rectangle inv_rec = {
            .x = 0,
            .y = 0,
            .width = rect.w,
            .height = -rect.h
        };
        Vector2 pos = {.x = 0, .y = 0};
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawTextureRec(impl.texture.texture, inv_rec, pos, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}