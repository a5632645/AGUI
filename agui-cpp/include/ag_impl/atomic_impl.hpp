/*
 * \file atomic_impl.h
 * \brief 原子依赖的实现
*/

#pragma once
#include <atomic>
#include "agui/ag_types.hpp"

namespace agui {

/* 原子操作 */
typedef std::atomic_bool ag_atomic_bool_t;
static ag_bool ag_atomic_bool_load(ag_atomic_bool_t& p) { return p.load(); }
static void ag_atomic_bool_store(ag_atomic_bool_t& p, ag_bool v) { p.store(v); }

typedef std::atomic_uint8_t ag_atomic_uint8_t;
static ag_uint8 ag_atomic_uint8_load(ag_atomic_uint8_t& p) { return p.load(); }
static void ag_atomic_uint8_store(ag_atomic_uint8_t& p, ag_uint8 v) { p.store(v); }
static ag_uint8 ag_atomic_uint8_inc(ag_atomic_uint8_t& p) { return ++p; }
static ag_uint8 ag_atomic_uint8_dec(ag_atomic_uint8_t& p) { return --p; }

}
