/*
 * \file painter_impl.h
 * \brief 在这里实现AgPainter接口，需要自己适配，可能要移出去
*/

#pragma once
#include "agui/ag_painter.h"
#include "raylib.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    AgPainter painter;
    RenderTexture2D texture;
} PainterImpl;

void PainterImpl_Init(PainterImpl* impl);

#ifdef __cplusplus
}
#endif
