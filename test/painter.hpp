#pragma once
#include "agui/ag_painter.hpp"
#include "ag_impl/draws_impl.hpp"
#include "raylib.h"

using namespace agui;
using namespace agui::impl;

static Color ColorTransfrom(agui::AgColor c) {
    return Color {
        c.r,
        c.g,
        c.b,
        c.a
    };
}

class TestPainter : public AgPainter {
public:
    NullablePtr<void> GetBackend() override {
        return nullptr;
    }

    auto GetTexture() {
        return texture_;
    }

    void CallDraw(const AgDraw& arg) override {
        ag_int16 x = draw_aera_.x;
        ag_int16 y = draw_aera_.y;
        switch (arg.GetType()) {
        case eAgDrawType_Fill: {
            auto& d = arg.As<AgFillDraw>();
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
            auto& d = arg.As<AgLineDraw>();
            Color c = ColorTransfrom(d.color);
            DrawLine(x + d.x1,
                    y + d.y1,
                    x + d.x2,
                    y + d.y2,
                    c);
        }
        break;
        case eAgDrawType_Pixel: {
            auto&d = arg.As<AgPixelDraw>();
            Color c = ColorTransfrom(d.color);
            DrawPixel(x + d.x,
                    y + d.y,
                    c);
        }
        break;
        case eAgDrawType_Rect: {
            auto& d = arg.As<AgRectDraw>();
            Color c = ColorTransfrom(d.color);
            DrawRectangleLines(x + d.rect.x,
                            y + d.rect.y,
                            d.rect.w,
                            d.rect.h,
                            c);
        }
        break;
        case eAgDrawType_Text: {
            auto& d = arg.As<AgTextDraw>();
                Color c = ColorTransfrom(d.color);
                ag_int16 text_width = MeasureText(d.text, d.height);
                ag_int16 text_height = d.height;
                ag_int16 x = d.rect.x + draw_aera_.x;
                ag_int16 y = d.rect.y + draw_aera_.y;

                AgAlignEnum x_align = AGUI_X_ALIGN(d.align);
                switch (x_align) {
                case eAgAlign_XLeft:
                    break;
                case eAgAlign_XCenter:
                    x += (d.rect.w - text_width) / 2;
                    break;
                case eAgAlign_XRight:
                    x += d.rect.w - text_width;
                    break;
                default:
                    break;
                }

                AgAlignEnum y_align = AGUI_Y_ALIGN(d.align);
                switch (y_align) {
                case eAgAlign_YTop:
                    break;
                case eAgAlign_YCenter:
                    y += (d.rect.h - text_height) / 2;
                    break;
                case eAgAlign_YBottom:
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