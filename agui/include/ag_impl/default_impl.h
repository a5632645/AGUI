/**
 * @file default_impl.h
 * @brief 需要实现的一些默认行为
 * @version 0.1
 * @date 2024-10-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "agui/ag_obj.h"
#include "agui/ag_painter.h"
#include "agui/ag_rect.h"
#include "agui/ag_color.h"

void __EmptyObjDraw(AgObj* obj, AgPainter* painter);
void __DrawColorFrame(AgPainter* painter, const AgRect* aera, AgColor color);
