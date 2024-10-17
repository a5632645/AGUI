/*
 * \file atomic_impl.h
 * \brief 原子依赖的实现
*/

#pragma once
#include <stdatomic.h>

/* 原子操作 */
typedef atomic_bool ag_atomic_bool_t;
#define AG_ATOMIC_BOOL_STORE(p, v) atomic_store(p, v)
#define AG_ATOMIC_BOOL_LOAD(p) atomic_load(p)