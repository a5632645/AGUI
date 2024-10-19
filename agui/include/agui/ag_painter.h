#pragma once
#include "agui/ag_types.h"
#include "agui/ag_color.h"
#include "agui/ag_rect.h"
#include "agui/ag_draw.h"
#include "ag_impl/ag_cfg.h"

typedef struct __AgPainter {
    // ---------------------------------------- interface ----------------------------------------
    void*(*get_backend)(struct __AgPainter* painter);                   /* 获取绘制后端，如u8g2,adfurit等 */
    void(*call_draw)(struct __AgPainter* painter, const AgDraw* arg);   /* 绘制东西 */
    void(*begin_frame)(struct __AgPainter* painter);                    /* 开始新的绘制 */
    void(*end_frame)(struct __AgPainter* painter);                      /* 结束绘制 */

    // ---------------------------------------- internal ----------------------------------------
    AgRect draw_aera;       /* 临时变量，代表全局绘制区域 */
    ag_bool always_redraw;  /* 标记永远绘制。当某个节点标记重绘时，该位被标记 */
    ag_int16 depth;         /* 递归迭代位置 */
    struct __PainterData {  /* 迭代保存数据 */
        AgRect draw_aera;
        ag_bool always_redraw;
    } data[AGUI_PAINTER_MAX_STEPS];
} AgPainter;

/**
 * @brief 初始化
 * @param painter 不能为NULL
 */
void AgPainter_Init(AgPainter* painter);

/**
 * @brief 获取全局空间的绘制区域
 * @param painter 不能为NULL
 * @return 
 */
const AgRect* AgPainter_GetDrawAera(AgPainter* painter);

/**
 * @brief 获取现在空间的绘制区域
 * @param painter 不能为NULL
 * @param draw_aera 不能为NULL
 */
void AgPainter_GetLocalDrawAera(AgPainter* painter, AgRect* draw_aera);

void AgPainter_SaveState(AgPainter* painter);
void AgPainter_RestoreState(AgPainter* painter);
void AgPainter_PrepareDraw(AgPainter* painter, const AgRect* global_aera);
