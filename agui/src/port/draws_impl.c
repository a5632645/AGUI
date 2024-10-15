#include "agui/port/draws_impl.h"

void PixelDraw_Init(PixelDraw* draw, AgPainter* painter) {
    draw->draw.clip = &painter->draw_aera;
    draw->draw.type = eAgDrawType_Pixel;
}

void LineDraw_Init(LineDraw* draw, AgPainter* painter) {
    draw->draw.clip = &painter->draw_aera;
    draw->draw.type = eAgDrawType_Line;
}

void RectDraw_Init(RectDraw* draw, AgPainter* painter) {
    draw->draw.clip = &painter->draw_aera;
    draw->draw.type = eAgDrawType_Rect;
}

void FillDraw_Init(FillDraw* draw, AgPainter* painter) {
    draw->draw.clip = &painter->draw_aera;
    draw->draw.type = eAgDrawType_Fill;
}

void TextDraw_Init(TextDraw* draw, AgPainter* painter) {
    draw->draw.clip = &painter->draw_aera;
    draw->draw.type = eAgDrawType_Text;
}

void ImgDraw_Init(ImgDraw* draw, AgPainter* painter) {
    draw->draw.clip = &painter->draw_aera;
    draw->draw.type = eAgDrawType_Img;
}