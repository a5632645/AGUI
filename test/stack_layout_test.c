#include "ag_impl/painter_impl.h"
#include "ag_impl/draws_impl.h"
#include "ag_comp/ag_stack_layout.h"
#include "raylib.h"
#include <stdio.h>

static void Obj1_Draw(AgObj* obj, AgPainter* painter) {
    AgTextDraw draw;
    AgTextDraw_Init(&draw);
    draw.text = "Hello World";
    draw.align = eAgAlign_Center;
    draw.color = AG_COLOR_WHITE;
    draw.font_size = 20;
    AgObj_GetLocalBound(obj, &draw.rect);
    painter->call_draw(painter, &draw.draw);
}

static void Obj2_Draw(AgObj* obj, AgPainter* painter) {
    AgTextDraw draw;
    AgTextDraw_Init(&draw);
    draw.text = "Hello obj2";
    draw.align = eAgAlign_Center;
    draw.color = AG_COLOR_WHITE;
    draw.font_size = 20;
    AgObj_GetLocalBound(obj, &draw.rect);
    painter->call_draw(painter, &draw.draw);
}

int main() {
    AgRect screen = {0, 0, 800, 600};
    InitWindow(800, 600, "raylib [examples] example - list view");
    SetTargetFPS(60);

    PainterImpl impl;
    PainterImpl_Init(&impl);

    /* objs */
    AgStackLayout sl;
    AgStackLayout_Init(&sl);
    AgObj_SetBound(&sl.obj, &screen);

    AgObj obj1;
    AgObj_Init(&obj1);
    obj1.vfunc.draw = Obj1_Draw;

    AgObj obj2;
    AgObj_Init(&obj2);
    obj2.vfunc.draw = Obj2_Draw;

    AgObj obj3;
    AgObj_Init(&obj3);
    obj3.vfunc.draw = Obj2_Draw;

    AgStackLayout_Push(&sl, &obj1);
    AgStackLayout_Push(&sl, &obj2);
    AgStackLayout_Push(&sl, &obj3);

    /* loop */
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) {
            static int i = 0;
            if (i == 0) {
                AgStackLayout_Push2(&sl, &obj1);
                i = 1;
            } else {
                AgStackLayout_Push2(&sl, &obj2);
                i = 0;
            }
        }
        if (IsKeyPressed(KEY_ENTER)) {
            AgList list;
            AgStackLayout_Push3(&sl, &list, &obj1);
            AgListNode* node = list.head;
            while (NULL != node) {
                AgObj* o = AGUI_CONTAINER_OF(AgObj, node, node);
                if (o == &obj1) {
                    printf("obj1\n");
                }
                if (o == &obj2) {
                    printf("obj2\n");
                }
                if (o == &obj3) {
                    printf("obj3\n");
                }
                node = node->next;
            }
        }

        if (sl.obj.flags.invalid) {
            AgObj_DrawObj(&sl.obj, &impl.painter);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        Vector2 pos = {.x = 0, .y = 0};
        Rectangle rect = {0 ,0, screen.w, -screen.h};
        DrawTextureRec(impl.texture.texture, rect, pos, WHITE);
        EndDrawing();
    }
    CloseWindow();
}