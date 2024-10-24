#include "ag_impl/painter_impl.hpp"
#include "ag_impl/draws_impl.hpp"

namespace agui { namespace impl {

// ---------------------------------------- private ----------------------------------------
static Color ColorTransfrom(AgColor color) {
    return (Color){color.r, color.g, color.b, color.a};
}

void PainterImpl::DrawText(const AgTextDraw& arg) {
    Color c = ColorTransfrom(arg.color);
    ag_int16 text_width = MeasureText(arg.text, arg.font_size);
    ag_int16 text_height = arg.font_size;
    ag_int16 x = arg.rect.x + draw_aera_.x;
    ag_int16 y = arg.rect.y + draw_aera_.y;

    AgAlignEnum x_align = AGUI_X_ALIGN(arg.align);
    switch (x_align) {
    case eAgAlign_XLeft:
        break;
    case eAgAlign_XCenter:
        x += (arg.rect.w - text_width) / 2;
        break;
    case eAgAlign_XRight:
        x += arg.rect.w - text_width;
        break;
    default:
        break;
    }

    AgAlignEnum y_align = AGUI_Y_ALIGN(arg.align);
    switch (y_align) {
    case eAgAlign_YTop:
        break;
    case eAgAlign_YCenter:
        y += (arg.rect.h - text_height) / 2;
        break;
    case eAgAlign_YBottom:
        y += arg.rect.h - text_height;
        break;
    default:
        break;
    }
    
    ::DrawText(arg.text, x, y, arg.font_size, c);
}

// ---------------------------------------- function impl ----------------------------------------
NullablePtr<void> PainterImpl::GetBackend() const {
    return NULL;
}

void PainterImpl::BeginFrame() {
    ::BeginTextureMode(texture_);
}

void PainterImpl::EndFrame() {
    ::EndTextureMode();
}

void PainterImpl::CallDraw(const AgDraw& arg) {
    ag_int16 x = draw_aera_.x;
    ag_int16 y = draw_aera_.y;
    switch (arg.type) {
    case eAgDrawType_Fill: {
        auto& draw = arg.As<AgFillDraw>();
        Color c = ColorTransfrom(draw.color);
        DrawRectangle(x + draw.rect.x,
                       y + draw.rect.y,
                       draw.rect.w,
                       draw.rect.h,
                       c);
    }
    break;
    case eAgDrawType_Img: {
        // TODO: img
    }
    break;
    case eAgDrawType_Line: {
        auto& draw = arg.As<AgLineDraw>();
        Color c = ColorTransfrom(draw.color);
        DrawLine(x + draw.x1,
                 y + draw.y1,
                 x + draw.x2,
                 y + draw.y2,
                 c);
    }
    break;
    case eAgDrawType_Pixel: {
        auto& draw = arg.As<AgPixelDraw>();
        Color c = ColorTransfrom(draw.color);
        DrawPixel(x + draw.x,
                  y + draw.y,
                  c);
    }
    break;
    case eAgDrawType_Rect: {
        auto& draw = arg.As<AgRectDraw>();
        Color c = ColorTransfrom(draw.color);
        DrawRectangleLines(x + draw.rect.x,
                           y + draw.rect.y,
                           draw.rect.w,
                           draw.rect.h,
                           c);
    }
    break;
    case eAgDrawType_Text: {
        auto& draw = arg.As<AgTextDraw>();
        DrawText(draw);
    }
    break;
    default:
        break;
    };
}


// ---------------------------------------- public ----------------------------------------
PainterImpl::PainterImpl() {
    int w = GetScreenWidth();
    int h = GetScreenHeight();
    texture_ = LoadRenderTexture(w, h);
}

PainterImpl::~PainterImpl() {
    UnloadRenderTexture(texture_);
}

} }