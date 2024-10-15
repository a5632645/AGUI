#pragma once
#include "agui/ag_types.h"
#include "agui/ag_color.h"
#include "agui/ag_rect.h"
#include "agui/ag_draw.h"

typedef struct __AgPainter {
    // ---------------------------------------- interface ----------------------------------------
    /* 获取绘制后端，如u8g2,adfurit等 */
    void*(*get_backend)(struct __AgPainter* painter);
    /* 绘制东西 */
    void(*call_draw)(struct __AgPainter* painter, const AgDraw* arg);
    /* 开始新的绘制 */
    void(*begin_frame)(struct __AgPainter* painter);
    /* 结束绘制 */
    void(*end_frame)(struct __AgPainter* painter);
    // ---------------------------------------- public ----------------------------------------
    /* 全局空间的绘制区域，可获取绘制后端后获得现在的绘制区域 */
    AgRect draw_aera;
    // ---------------------------------------- internal ----------------------------------------
    /* 标记永远绘制。当某个节点标记重绘时，该位被标记 */
    ag_bool always_redraw;
} AgPainter;