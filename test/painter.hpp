#pragma once
#include "agui/ag_painter.hpp"
#include "draws.hpp"
#include "raylib.h"

static Color ColorTransfrom(agui::AgColor c) {
    return Color {
        c.r,
        c.g,
        c.b,
        c.a
    };
}

class TestPainter : public agui::AgPainter {
public:
    agui::NullablePtr<void> GetBackend() override {
        return nullptr;
    }

    auto GetTexture() {
        return texture_;
    }

    void CallDraw(const agui::AgDraw& arg) override {
        agui::ag_int16 x = draw_aera_.x;
        agui::ag_int16 y = draw_aera_.y;
        switch (arg.GetType()) {
        case eAgDrawType_Fill: {
            auto& d = arg.As<FillDraw>();
            Color c = ColorTransfrom(d.color);
            DrawRectangle(x + d.rect.x,
                        y + d.rect.y,
                        d.rect.w,
                        d.rect.h,
                        c);
        }
        break;
        case eAgDrawType_Img: {
            // TODO: img
        }
        break;
        case eAgDrawType_Line: {
            auto& d = arg.As<LineDraw>();
            Color c = ColorTransfrom(d.color);
            DrawLine(x + d.x1,
                    y + d.y1,
                    x + d.x2,
                    y + d.y2,
                    c);
        }
        break;
        case eAgDrawType_Pixel: {
            auto&d = arg.As<PixelDraw>();
            Color c = ColorTransfrom(d.color);
            DrawPixel(x + d.x,
                    y + d.y,
                    c);
        }
        break;
        case eAgDrawType_Rect: {
            auto& d = arg.As<RectDraw>();
            Color c = ColorTransfrom(d.color);
            DrawRectangleLines(x + d.rect.x,
                            y + d.rect.y,
                            d.rect.w,
                            d.rect.h,
                            c);
        }
        break;
        case eAgDrawType_Text: {
            auto& d = arg.As<TextDraw>();
                Color c = ColorTransfrom(d.color);
                agui::ag_int16 text_width = MeasureText(d.text, d.height);
                agui::ag_int16 text_height = d.height;
                agui::ag_int16 x = d.rect.x + draw_aera_.x;
                agui::ag_int16 y = d.rect.y + draw_aera_.y;

                agui::AgAlignEnum x_align = AGUI_X_ALIGN(d.align);
                switch (x_align) {
                case agui::AgAlignEnum::eAgAlign_XLeft:
                    break;
                case agui::AgAlignEnum::eAgAlign_XCenter:
                    x += (d.rect.w - text_width) / 2;
                    break;
                case agui::AgAlignEnum::eAgAlign_XRight:
                    x += d.rect.w - text_width;
                    break;
                default:
                    break;
                }

                agui::AgAlignEnum y_align = AGUI_Y_ALIGN(d.align);
                switch (y_align) {
                case agui::AgAlignEnum::eAgAlign_YTop:
                    break;
                case agui::AgAlignEnum::eAgAlign_YCenter:
                    y += (d.rect.h - text_height) / 2;
                    break;
                case agui::AgAlignEnum::eAgAlign_YBottom:
                    y += d.rect.h - text_height;
                    break;
                default:
                    break;
                }
                
                DrawText(d.text, x, y, d.height, c);
        }
        break;
        default:
            break;
        };
    }

    void BeginFrame() override {
        BeginTextureMode(texture_);
    }

    void EndFrame() override {
        EndTextureMode();
    }

    TestPainter() {
        auto w = GetScreenWidth();
        auto h = GetScreenHeight();
        texture_ = LoadRenderTexture(w, h);
    }

    ~TestPainter() override {
        UnloadRenderTexture(texture_);
    }
private:
    RenderTexture2D texture_;
};