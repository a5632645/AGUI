#pragma once
#include "agui/ag_types.h"
#include "agui/ag_color.h"
#include "agui/ag_rect.h"
#include "agui/ag_draw.h"

typedef struct __AgPainter {
    /* 获取绘制后端，如u8g2,adfurit等 */
    void*(*get_backend)(struct __AgPainter* painter);
    /* 设置颜色 */
    void(*set_color)(struct __AgPainter* painter, AgColor color);
    /* 绘制东西 */
    void(*call_draw)(struct __AgPainter* painter, const AgDraw* arg);
    /* 开始新的绘制 */
    void(*begin_frame)(struct __AgPainter* painter);
    /* 结束绘制 */
    void(*end_frame)(struct __AgPainter* painter);
    /* 获取绘制区域 */
    AgRect draw_aera;
    // ---------------------------------------- internal ----------------------------------------
    /* 标记永远绘制 */
    ag_bool always_redraw;
} AgPainter;