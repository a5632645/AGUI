/**
 * @file ag_screen.h
 * @brief 
 * @version 0.1
 * @date 2024-10-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#include "agui/ag_painter.h"
#include "agui/ag_list.h"

typedef enum {
    eAgScreenStatus_Ready = 0,
    eAgScreenStatus_Running,
    eAgScreenStatus_Paused,
} AgScreenStatus;

typedef struct __AgScreen {
    AgListNode node;
    void(*init)(struct __AgScreen*);
    void(*destory)(struct __AgScreen*);
    void(*pause)(struct __AgScreen*);
    void(*resume)(struct __AgScreen*);
    void(*draw_screen)(struct __AgScreen*, AgPainter*);
} AgScreen;