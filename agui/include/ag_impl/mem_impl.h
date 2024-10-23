/*
 * \file mem_impl.h
 * \brief 内存依赖的实现
 */

#pragma once
#include "agui/ag_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 内存 */
void* AgImpl_MemAlloc(ag_size_t size);
void AgImpl_MemFree(void* mem);

#ifdef __cplusplus
}
#endif
