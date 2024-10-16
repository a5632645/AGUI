#pragma once
#include "ag_comp/deps.h"

typedef struct {
    /* interface */
    AgObj obj;
    /* data */
    ag_bool press;
    const char* text;
    AgColor color;
    ag_uint32 font_size;
} AgButton;

void AgButton_Init(AgButton* obj);

/**
 * @brief 设置按钮的样式，不会触发消息
 * @param btn 
 * @param press 
 */
void AgButton_SetPress(AgButton* btn, ag_bool press);