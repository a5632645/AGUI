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

#include "agui/ag_obj.hpp"
#include "agui/ag_painter.hpp"
#include "agui/ag_rect.hpp"
#include "agui/ag_color.hpp"

namespace agui {

extern void __EmptyObjDraw(AgObj& obj, AgPainter& painter);
extern void __DrawColorFrame(AgPainter& painter, const AgRect& aera, AgColor color);

}
