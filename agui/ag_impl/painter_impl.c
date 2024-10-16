#include "ag_impl/painter_impl.h"
#include "ag_impl/draws_impl.h"

// ---------------------------------------- private ----------------------------------------
static Color ColorTransfrom(AgColor color) {
    return (Color){color.r, color.g, color.b, color.a};
}

static void AgDrawText(PainterImpl* impl, const TextDraw* arg) {
    Color c = ColorTransfrom(arg->color);
    ag_int16 text_width = MeasureText(arg->text, arg->font_size);
    ag_int16 text_height = arg->font_size;
    ag_int16 x = arg->rect.x + impl->painter.draw_aera.x;
    ag_int16 y = arg->rect.y + impl->painter.draw_aera.y;

    AgAlignEnum x_align = AGUI_X_ALIGN(arg->align);
    switch (x_align) {
    case eAgAlign_XLeft:
        break;
    case eAgAlign_XCenter:
        x += (arg->rect.w - text_width) / 2;
        break;
    case eAgAlign_XRight:
        x += arg->rect.w - text_width;
        break;
    default:
        break;
    }

    AgAlignEnum y_align = AGUI_Y_ALIGN(arg->align);
    switch (y_align) {
    case eAgAlign_YTop:
        break;
    case eAgAlign_YCenter:
        y += (arg->rect.h - text_height) / 2;
        break;
    case eAgAlign_YBottom:
        y += arg->rect.h - text_height;
        break;
    default:
        break;
    }
    
    DrawText(arg->text, x, y, arg->font_size, c);
}

// ---------------------------------------- function impl ----------------------------------------
static void* GetBackend(AgPainter* painter) {
    PainterImpl* impl = AGUI_CONTAINER_OF(PainterImpl, painter, painter);
    return NULL;
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
        Color c = ColorTransfrom(draw->color);
        DrawRectangle(x + draw->rect.x,
                       y + draw->rect.y,
                       draw->rect.w,
                       draw->rect.h,
                       c);
    }
    break;
    case eAgDrawType_Img: {
        // TODO: img
    }
    break;
    case eAgDrawType_Line: {
        LineDraw* draw = (LineDraw*)arg;
        Color c = ColorTransfrom(draw->color);
        DrawLine(x + draw->x1,
                 y + draw->y1,
                 x + draw->x2,
                 y + draw->y2,
                 c);
    }
    break;
    case eAgDrawType_Pixel: {
        PixelDraw* draw = (PixelDraw*)arg;
        Color c = ColorTransfrom(draw->color);
        DrawPixel(x + draw->x,
                  y + draw->y,
                  c);
    }
    break;
    case eAgDrawType_Rect: {
        RectDraw* draw = (RectDraw*)arg;
        Color c = ColorTransfrom(draw->color);
        DrawRectangleLines(x + draw->rect.x,
                           y + draw->rect.y,
                           draw->rect.w,
                           draw->rect.h,
                           c);
    }
    break;
    case eAgDrawType_Text: {
        TextDraw* draw = (TextDraw*)arg;
        AgDrawText(impl, draw);
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
    impl->painter.call_draw = CallDraw;
}