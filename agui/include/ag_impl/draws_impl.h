/*
 * \file draws_impl.h
 * \brief 在这里定义各种绘制
 */

#pragma once
#include "agui/ag_draw.h"
#include "agui/ag_painter.h"
#include "agui/ag_color.h"
#include "agui/ag_align.h"

/* 最好不要动 */
enum {
    eAgDrawType_Pixel = 0,
    eAgDrawType_Line,
    eAgDrawType_Rect,
    eAgDrawType_Fill,
    eAgDrawType_Text,
    eAgDrawType_Img
};

typedef struct {
    AgDraw draw;
    ag_int16 x;
    ag_int16 y;
    AgColor color;
} PixelDraw;
void PixelDraw_Init(PixelDraw* draw, AgPainter* painter);

typedef struct {
    AgDraw draw;
    ag_int16 x1;
    ag_int16 y1;
    ag_int16 x2;
    ag_int16 y2;
    AgColor color;
} LineDraw;
void LineDraw_Init(LineDraw* draw, AgPainter* painter);

typedef struct {
    AgDraw draw;
    AgRect rect;
    AgColor color;
} RectDraw;
void RectDraw_Init(RectDraw* draw, AgPainter* painter);

typedef struct {
    AgDraw draw;
    AgRect rect;
    AgColor color;
} FillDraw;
void FillDraw_Init(FillDraw* draw, AgPainter* painter);

typedef struct {
    AgDraw draw;
    const char* text;
    AgRect rect;
    ag_uint32 font_size;
    AgAlignEnum align;
    AgColor color;
} TextDraw;
void TextDraw_Init(TextDraw* draw, AgPainter* painter);

typedef struct {
    AgDraw draw;
    const void* img;
    ag_int16 x;
    ag_int16 y;
    AgColor color;
} ImgDraw;
void ImgDraw_Init(ImgDraw* draw, AgPainter* painter);