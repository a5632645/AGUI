#pragma once
#include "agui/ag_draw.h"
#include "agui/ag_painter.h"

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
} PixelDraw;
void PixelDraw_Init(PixelDraw* draw, AgPainter* painter);

typedef struct {
    AgDraw draw;
    ag_int16 x1;
    ag_int16 y1;
    ag_int16 x2;
    ag_int16 y2;
} LineDraw;
void LineDraw_Init(LineDraw* draw, AgPainter* painter);

typedef struct {
    AgDraw draw;
    AgRect rect;
} RectDraw;
void RectDraw_Init(RectDraw* draw, AgPainter* painter);

typedef struct {
    AgDraw draw;
    AgRect rect;
} FillDraw;
void FillDraw_Init(FillDraw* draw, AgPainter* painter);

typedef struct {
    AgDraw draw;
    const char* text;
    ag_int16 x;
    ag_int16 y;
    ag_uint32 font_size;
} TextDraw;
void TextDraw_Init(TextDraw* draw, AgPainter* painter);

typedef struct {
    AgDraw draw;
    const void* img;
    ag_int16 x;
    ag_int16 y;
} ImgDraw;
void ImgDraw_Init(ImgDraw* draw, AgPainter* painter);