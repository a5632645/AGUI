/*
 * \file draws_impl.h
 * \brief 在这里定义各种绘制，不要修改，可以增加
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
    eAgDrawType_Img,
    eAgDrawType_Invert,
    eAgDrawType_NumSystem
    /* 可以添加自定义 */
};

typedef struct {
    AgDraw draw;
    ag_int16 x;
    ag_int16 y;
    AgColor color;
} AgPixelDraw;
void AgPixelDraw_Init(AgPixelDraw* draw, AgPainter* painter);

typedef struct {
    AgDraw draw;
    ag_int16 x1;
    ag_int16 y1;
    ag_int16 x2;
    ag_int16 y2;
    AgColor color;
} AgLineDraw;
void AgLineDraw_Init(AgLineDraw* draw, AgPainter* painter);

typedef struct {
    AgDraw draw;
    AgRect rect;
    AgColor color;
} AgRectDraw;
void AgRectDraw_Init(AgRectDraw* draw, AgPainter* painter);

typedef struct {
    AgDraw draw;
    AgRect rect;
    AgColor color;
} AgFillDraw;
void AgFillDraw_Init(AgFillDraw* draw, AgPainter* painter);

typedef struct {
    AgDraw draw;
    const char* text;
    AgRect rect;
    ag_uint32 font_size;
    AgAlignEnum align;
    AgColor color;
} AgTextDraw;
void AgTextDraw_Init(AgTextDraw* draw, AgPainter* painter);

typedef struct {
    AgDraw draw;
    const void* img;
    ag_int16 x;
    ag_int16 y;
    AgColor color;
} AgImgDraw;
void AgImgDraw_Init(AgImgDraw* draw, AgPainter* painter);

typedef struct {
    AgDraw draw;
    AgRect rect;
} AgInvertDraw;
void AgInvertDraw_Init(AgInvertDraw* draw, AgPainter* painter);