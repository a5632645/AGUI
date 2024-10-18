#include "raylib.h"
#include "agui/ag_rect.h"
#include "ag_impl/painter_impl.h"
#include "agui/ag_obj.h"
#include "ag_key/ag_key.h"
#include "ag_impl/draws_impl.h"
#include "ag_comp/text_view.h"
#include "ag_comp/ag_stack_layout.h"

// ---------------------------------------- event ----------------------------------------
typedef struct {
    /* interface */
    AgEvent base;
    /* data */
    KeyboardKey key;
    ag_bool down;
} KeyEvent;
#define KEY_EVENT_OF(EVENT) AGUI_CONTAINER_OF(KeyEvent, base, EVENT)

enum {
    eAgEvent_Key = 0
};

// --------------------------------------------------------------------------------
// objects
// --------------------------------------------------------------------------------
// ---------------------------------------- 顶部切换条 ----------------------------------------
typedef struct {
    /* interface */
    AgObj obj;
    /* data */
    AgTextView osc;
    AgTextView filter;
} TopBar;
static void TopBar_Draw(AgObj* obj, AgPainter* painter) {
    TopBar* tp = AGUI_CONTAINER_OF(TopBar, obj, obj);

    AgRectDraw draw = {
        .color = AG_COLOR_WHITE
    };
    AgRect_Copy(&draw.rect, &tp->osc.obj.bound);
    painter->call_draw(painter, &draw.draw);

    AgRect_Copy(&draw.rect, &tp->filter.obj.bound);
    painter->call_draw(painter, &draw.draw);
}
static void TopBar_Init(TopBar* tp) {
    AgObj_Init(&tp->obj);
    tp->obj.vfunc.draw = TopBar_Draw;

    AgTextView_Init(&tp->osc);
    tp->osc.text = "osc";
    tp->osc.align = eAgAlign_Center;
    AgObj_SetBounds(&tp->osc.obj, 0, 0, 100, 40);

    AgTextView_Init(&tp->filter);
    tp->filter.text = "filter";
    tp->filter.align = eAgAlign_Center;
    AgObj_SetBounds(&tp->filter.obj, 100, 0, 100, 40);

    /* add */
    AgObj_AddChild(&tp->obj, &tp->osc.obj);
    AgObj_AddChild(&tp->obj, &tp->filter.obj);
}

// --------------------------------------------------------------------------------
// 底部
// --------------------------------------------------------------------------------
// ---------------------------------------- osc布局 ----------------------------------------
typedef struct {
    AgObj obj;
} OscPanel;
static void OscPanel_Draw(AgObj* obj, AgPainter* painter) {
    
}
static void OscPanel_Init(OscPanel* osc) {
    AgObj_Init(&osc->obj);
    osc->obj.vfunc.draw = OscPanel_Draw;
}

// ---------------------------------------- filter布局 ----------------------------------------
typedef struct {
    AgObj obj;
} FilterPanel;
static void FilterPanel_Draw(AgObj* obj, AgPainter* painter) {
    
}
static void FilterPanel_Init(FilterPanel* filter) {
    AgObj_Init(&filter->obj);
    filter->obj.vfunc.draw = FilterPanel_Draw;
}

// ---------------------------------------- 底部切换布局 ----------------------------------------
typedef struct {
    AgStackLayout sl;
    OscPanel osc;
    FilterPanel filter;
} BottomLayout;
static void BottomLayout_Init(BottomLayout* bl) {
    AgStackLayout_Init(&bl->sl);
    OscPanel_Init(&bl->osc);
    FilterPanel_Init(&bl->filter);
    AgStackLayout_Push(&bl->sl, &bl->osc.obj);
}

// ---------------------------------------- global ----------------------------------------
typedef struct {
    /* interface */
    AgObj obj;
    /* data */
    TopBar top;
    BottomLayout bottom;
} Root;
static void Root_Layout(AgObj* obj) {
    Root* root = AGUI_CONTAINER_OF(Root, obj, obj);
    AgObj_SetBounds(&root->top.obj, 0, 0, obj->bound.w, 40);
    AgObj_SetBounds(&root->bottom.sl.obj, 0, 40, obj->bound.w, obj->bound.h - 40);
}
static void Root_Init(Root* root) {
    AgObj_Init(&root->obj);
    root->obj.vfunc.layout = Root_Layout;

    TopBar_Init(&root->top);
    BottomLayout_Init(&root->bottom);

    AgObj_AddChild(&root->obj, &root->top.obj);
    AgObj_AddChild(&root->obj, &root->bottom.sl.obj);
}

static AgKeySwitcher ks;
static Root root;

// ---------------------------------------- func declares ----------------------------------------
static void TopBar_SetSwitcher(AgKeySwitcher* ks);
static void TopBar_SwitchEvent(AgKeySwitcher* ks, AgEvent* event);

// ---------------------------------------- main ----------------------------------------
int main(void) {
    AgRect screen = {0, 0, 800, 600};
    InitWindow(screen.w, screen.h, "agui");

    PainterImpl impl;
    PainterImpl_Init(&impl);

    Root_Init(&root);
    AgObj_SetBound(&root.obj, &screen);
    AgKeySwitcher_Init(&ks, &root.obj);
    TopBar_SetSwitcher(&ks);

    /* main loop */
    while (!WindowShouldClose()) {
        /* input */
        int key = GetKeyPressed();
        while (key != 0) {
            KeyEvent key_e = {
                .base = {
                    .handled = ag_false,
                    .sender = NULL,
                    .type = eAgEvent_Key
                },
                .key = key,
                .down = ag_true
            };
            AgKeySwitcher_SendEvent(&ks, &key_e.base);

            if (IsKeyReleased(key)) {
                key_e.base.handled = ag_false;
                key_e.down = ag_false;
                key_e.base.sender = NULL;
                AgKeySwitcher_SendEvent(&ks, &key_e.base);
            }

            key = GetKeyPressed();
        }

        /* update */
        if (ag_true == root.obj.flags.invalid) {
            AgObj_DrawObj(&root.obj, &impl.painter);
        }

        /* draw buffer */
        Rectangle inv_rec = {
            .x = 0,
            .y = 0,
            .width = screen.w,
            .height = -screen.h
        };
        Vector2 pos = {.x = 0, .y = 0};
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawTextureRec(impl.texture.texture, inv_rec, pos, WHITE);
        EndDrawing();
    }
}

// ---------------------------------------- func ----------------------------------------
static void TopBar_SetSwitcher(AgKeySwitcher* ks) {
    Root* root = AGUI_CONTAINER_OF(Root, obj, ks->current);
    ks->event = TopBar_SwitchEvent;
    AgKeySwitcher_Goto(ks, Agobj_FirstChild(&root->top.obj));
}

static void TopBar_SwitchEvent(AgKeySwitcher* ks, AgEvent* event) {
    if (event->type != eAgEvent_Key) {
        return;
    }

    KeyEvent* e = KEY_EVENT_OF(event);
    if (ag_false == e->down) {
        return;
    }

    event->handled = ag_true;
    switch (e->key) {
    case KEY_LEFT: {
        AgKeySwitcher_GoPrev(ks);
        break;
    }
    case KEY_RIGHT: {
        AgKeySwitcher_GoNext(ks);
        break;
    }
    case KEY_DOWN: {
        
        break;
    }
    default:
        break;
    }
}