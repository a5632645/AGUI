#include "agui/port/painter_impl.h"
#include "agui/port/draws_impl.h"


static void* GetBackend(AgPainter* painter) {
    PainterImpl* impl = AGUI_CONTAINER_OF(PainterImpl, painter, painter);
    return NULL;
}

static void SetColor(AgPainter* painter, AgColor color) {
    PainterImpl* impl = AGUI_CONTAINER_OF(PainterImpl, painter, painter);
    impl->color.r = color.r;
    impl->color.g = color.g;
    impl->color.b = color.b;
    impl->color.a = color.a;
}

static void BeginFrame(AgPainter* painter) {
    BeginDrawing();
}

static void EndFrame(AgPainter* painter) {
    EndDrawing();
}

static void CallDraw(AgPainter* painter, const AgDraw* arg) {
    PainterImpl* impl = AGUI_CONTAINER_OF(PainterImpl, painter, painter);
    ag_int16 x = painter->draw_aera.x;
    ag_int16 y = painter->draw_aera.y;
    switch (arg->type) {
    case eAgDrawType_Fill: {
        FillDraw* draw = (FillDraw*)arg;
        DrawRectangle(x + draw->rect.x,
                       y + draw->rect.y,
                       draw->rect.w,
                       draw->rect.h,
                       impl->color);
    }
    break;
    case eAgDrawType_Img: {
        // TODO: img
    }
    break;
    case eAgDrawType_Line: {
        LineDraw* draw = (LineDraw*)arg;
        DrawLine(x + draw->x1,
                 y + draw->y1,
                 x + draw->x2,
                 y + draw->y2,
                 impl->color);
    }
    break;
    case eAgDrawType_Pixel: {
        PixelDraw* draw = (PixelDraw*)arg;
        DrawPixel(x + draw->x,
                  y + draw->y,
                  impl->color);
    }
    break;
    case eAgDrawType_Rect: {
        RectDraw* draw = (RectDraw*)arg;
        DrawRectangleLines(x + draw->rect.x,
                           y + draw->rect.y,
                           draw->rect.w,
                           draw->rect.h,
                           impl->color);
    }
    break;
    case eAgDrawType_Text: {
        TextDraw* draw = (TextDraw*)arg;
        DrawText(draw->text,
                 x + draw->x,
                 y + draw->y,
                 draw->font_size,
                 impl->color);
    }
    break;
    default:
        break;
    };
}


// ---------------------------------------- public ----------------------------------------
void PainterImpl_Init(PainterImpl* impl) {
    impl->painter.begin_frame = BeginFrame;
    impl->painter.end_frame = EndFrame;
    impl->painter.get_backend = GetBackend;
    impl->painter.set_color = SetColor;
    impl->painter.call_draw = CallDraw;
}