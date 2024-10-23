/*
 * @file ag_cfg.h
 * @brief agui config
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*
 * @brief obj的边框没有重叠，包括覆盖！如果存在覆盖，必须刷新整个屏幕
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

#ifdef __cplusplus
}
#endif
