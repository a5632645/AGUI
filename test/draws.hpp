/*
 * \file draws_impl.h
 * \brief 在这里定义各种绘制，不依赖该文件，可能要移动出去
 */

#pragma once
#include "agui/ag_draw.hpp"
#include "agui/ag_color.hpp"
#include "agui/ag_align.hpp"
#include "agui/ag_rect.hpp"

enum {
    eAgDrawType_Fill = 0,
    eAgDrawType_Rect,
    eAgDrawType_Pixel,
    eAgDrawType_Line,
    eAgDrawType_Text,
    eAgDrawType_Img,
    eAgDrawType_Invert,
};

struct FillDraw : public agui::AgDraw {
    FillDraw() : AgDraw(eAgDrawType_Fill) {}
    agui::AgRect rect;
    agui::AgColor color;
};

struct RectDraw : public agui::AgDraw {
    RectDraw() : AgDraw(eAgDrawType_Rect) {}
    agui::AgRect rect;
    agui::AgColor color;
};

struct LineDraw : public agui::AgDraw {
    LineDraw() : AgDraw(eAgDrawType_Line) {}
    agui::ag_int16 x1;
    agui::ag_int16 y1;
    agui::ag_int16 x2;
    agui::ag_int16 y2;
    agui::AgColor color;
};

struct TextDraw : public agui::AgDraw {
    TextDraw() : AgDraw(eAgDrawType_Text) {}
    agui::AgRect rect;
    agui::AgAlignEnum align;
    const char *text;
    agui::AgColor color;
    agui::ag_uint8 height;
};

struct ImgDraw : public agui::AgDraw {
    ImgDraw() : AgDraw(eAgDrawType_Img) {}
    agui::AgRect rect;
    agui::AgColor color;
};

struct PixelDraw : public agui::AgDraw {
    PixelDraw() : AgDraw(eAgDrawType_Pixel) {}
    agui::ag_int16 x;
    agui::ag_int16 y;
    agui::AgColor color;
};
