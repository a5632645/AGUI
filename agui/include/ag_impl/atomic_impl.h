/*
 * \file atomic_impl.h
 * \brief 原子依赖的实现
*/

#pragma once
#include <stdatomic.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 原子操作 */
typedef atomic_bool ag_atomic_bool_t;
#define AG_ATOMIC_BOOL_STORE(p, v) atomic_store(p, v)
#define AG_ATOMIC_BOOL_LOAD(p) atomic_load(p)

typedef atomic_int ag_atomic_int_t;
#define AG_ATOMIC_INT_STORE(p, v) atomic_store(p, v)
#define AG_ATOMIC_INT_LOAD(p) atomic_load(p)
#define AG_ATOMIC_INT_ADD(p, v) atomic_fetch_add(p, v)
#define AG_ATOMIC_INT_SUB(p, v) atomic_fetch_sub(p, v)

#ifdef __cplusplus
}
#endif
