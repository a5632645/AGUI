/*
 * \file draws_impl.h
 * \brief 在这里定义各种绘制，不依赖该文件，可能要移动出去
 */

#pragma once
#include "agui/ag_draw.h"
#include "agui/ag_color.h"
#include "agui/ag_align.h"

enum {
    eAgDrawType_Fill = 0,
    eAgDrawType_Rect,
    eAgDrawType_Pixel,
    eAgDrawType_Line,
    eAgDrawType_Text,
    eAgDrawType_Img,
    eAgDrawType_Invert,
};

typedef struct {
    AgDraw draw;
    AgRect rect;
    AgColor color;
} AgFillDraw;
void AgFillDraw_Init(AgFillDraw* draw);

typedef struct {
    AgDraw draw;
    AgRect rect;
    AgColor color;
} AgRectDraw;
void AgRectDraw_Init(AgRectDraw* draw);

// ---------------------------------------- custom ----------------------------------------
typedef struct {
    AgDraw draw;
    ag_int16 x;
    ag_int16 y;
    AgColor color;
} AgPixelDraw;
void AgPixelDraw_Init(AgPixelDraw* draw);

typedef struct {
    AgDraw draw;
    ag_int16 x1;
    ag_int16 y1;
    ag_int16 x2;
    ag_int16 y2;
    AgColor color;
} AgLineDraw;
void AgLineDraw_Init(AgLineDraw* draw);

typedef struct {
    AgDraw draw;
    const char* text;
    AgRect rect;
    ag_uint32 font_size;
    AgAlignEnum align;
    AgColor color;
} AgTextDraw;
void AgTextDraw_Init(AgTextDraw* draw);

typedef struct {
    AgDraw draw;
    const void* img;
    ag_int16 x;
    ag_int16 y;
    AgColor color;
} AgImgDraw;
void AgImgDraw_Init(AgImgDraw* draw);

typedef struct {
    AgDraw draw;
    AgRect rect;
} AgInvertDraw;
void AgInvertDraw_Init(AgInvertDraw* draw);