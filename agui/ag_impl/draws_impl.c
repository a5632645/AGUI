#include "ag_impl/draws_impl.h"

void AgPixelDraw_Init(AgPixelDraw* draw) {
    draw->draw.type = eAgDrawType_Pixel;
}

void AgLineDraw_Init(AgLineDraw* draw) {
    draw->draw.type = eAgDrawType_Line;
}

void AgRectDraw_Init(AgRectDraw* draw) {
    draw->draw.type = eAgDrawType_Rect;
}

void AgFillDraw_Init(AgFillDraw* draw) {
    draw->draw.type = eAgDrawType_Fill;
}

void AgTextDraw_Init(AgTextDraw* draw) {
    draw->draw.type = eAgDrawType_Text;
}

void AgImgDraw_Init(AgImgDraw* draw) {
    draw->draw.type = eAgDrawType_Img;
}

void AgInvertDraw_Init(AgInvertDraw* draw) {
    draw->draw.type = eAgDrawType_Invert;
}