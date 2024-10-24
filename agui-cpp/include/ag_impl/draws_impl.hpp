/*
 * \file draws_impl.h
 * \brief 在这里定义各种绘制，不依赖该文件，可能要移动出去
 */

#pragma once
#include "agui/ag_draw.hpp"
#include "agui/ag_color.hpp"
#include "agui/ag_align.hpp"

namespace agui::impl {

enum {
    eAgDrawType_Fill = 0,
    eAgDrawType_Rect,
    eAgDrawType_Pixel,
    eAgDrawType_Line,
    eAgDrawType_Text,
    eAgDrawType_Img,
    eAgDrawType_Invert,
};

struct AgFillDraw : public AgDraw {
    AgFillDraw() : AgDraw(eAgDrawType_Fill) {}
    AgRect rect;
    AgColor color;
};

struct AgRectDraw : public AgDraw {
    AgRectDraw() : AgDraw(eAgDrawType_Rect) {}
    AgRect rect;
    AgColor color;
};

struct AgLineDraw : public AgDraw {
    AgLineDraw() : AgDraw(eAgDrawType_Line) {}
    ag_int16 x1;
    ag_int16 y1;
    ag_int16 x2;
    ag_int16 y2;
    AgColor color;
};

struct AgTextDraw : public AgDraw {
    AgTextDraw() : AgDraw(eAgDrawType_Text) {}
    AgRect rect;
    AgAlignEnum align;
    const char *text;
    AgColor color;
    ag_int32 font_size;
};

struct AgImgDraw : public AgDraw {
    AgImgDraw() : AgDraw(eAgDrawType_Img) {}
    AgRect rect;
    AgColor color;
};

struct AgPixelDraw : public AgDraw {
    AgPixelDraw() : AgDraw(eAgDrawType_Pixel) {}
    ag_int16 x;
    ag_int16 y;
    AgColor color;
};

}
