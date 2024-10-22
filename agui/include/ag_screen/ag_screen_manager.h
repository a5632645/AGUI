/**
 * @file ag_screen_manager.h
 * @brief 
 * @version 0.1
 * @date 2024-10-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#include "agui/ag_list.h"
#include "ag_screen/ag_screen.h"

typedef struct {
    AgList screen_stack;
} AgScreenManager;

void AgScreenManager_Init(AgScreenManager* manager);
void AgScreenManager_Destory(AgScreenManager* manager);
void AgScreenManager_PushScreen(AgScreenManager* manager, AgScreen* screen);
void AgScreenManager_PopScreen(AgScreenManager* manager);
void AgScreenManager_Update(AgScreenManager* manager);