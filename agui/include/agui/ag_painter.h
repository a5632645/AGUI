#pragma once
#include "agui/ag_types.h"
#include "agui/ag_color.h"
#include "agui/ag_rect.h"
#include "agui/ag_draw.h"
#include "ag_impl/ag_cfg.h"

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
    // ---------------------------------------- internal ----------------------------------------
    /* 临时变量，代表全局绘制区域 */
    AgRect draw_aera;
    /* 标记永远绘制。当某个节点标记重绘时，该位被标记 */
    ag_bool always_redraw;
    /* 递归迭代位置 */
    ag_int16 depth;
    /* 迭代保存数据 */
    struct __PainterData {
        AgRect draw_aera;
        ag_bool always_redraw;
    } data[AGUI_PAINTER_MAX_STEPS];
} AgPainter;

/**
 * @brief 初始化
 * @param painter 
 */
void AgPainter_Init(AgPainter* painter);

/**
 * @brief 获取全局空间的绘制区域
 * @param painter 
 * @return 
 */
const AgRect* AgPainter_GetDrawAera(AgPainter* painter);

/**
 * @brief 获取全局空间的裁剪区域
 * @param painter 
 * @return 
 */
const AgRect* AgPainter_GetClipAera(AgPainter* painter);

void AgPainter_SaveState(AgPainter* painter);
void AgPainter_RestoreState(AgPainter* painter);
void AgPainter_PrepareDraw(AgPainter* painter, const AgRect* global_aera);
