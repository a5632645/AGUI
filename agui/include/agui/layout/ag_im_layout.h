#pragma once
#include "agui/ag_obj.h"
#include "agui/ag_align.h"

/**
 * @brief 简单的操作布局，仅在vfun.layout有效
 * @param obj 不能为NULL
 * @param align
 */
void AgImLayout_Layout(AgObj* obj, AgAlignEnum align);