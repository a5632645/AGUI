/*
 * @file ag_cfg.h
 * @brief agui config
 */

#pragma once

/*
 * @brief obj的边框没有重叠，如果发生了覆盖，处于底部的obj更新时顶部的obj必须调用agobj_redraw
 */
#define AGUI_NO_OBJ_OVERLAY 1

/*
 * @brief agui painter的最大递归层级
*/
#define AGUI_PAINTER_MAX_STEPS 32

/*
 * @brief debug的obj边框绘制
*/
#define AGUI_DBG_WIRE_FRAME 1
