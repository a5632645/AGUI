#pragma once
#include <atomic>
#include <mutex>
#include "agui/ag_types.hpp"

namespace agui {

class AgObj;
class AgPainter;
struct AgRect;
struct AgColor;

}

namespace agui::impl {

/* 原子操作 */
using ag_atomic_bool_t = std::atomic_bool;
static ag_bool ag_atomic_bool_load(ag_atomic_bool_t& p) { return p.load(); }
static void ag_atomic_bool_store(ag_atomic_bool_t& p, ag_bool v) { p.store(v); }

using ag_atomic_uint8_t = std::atomic_uint8_t;
static ag_uint8 ag_atomic_uint8_load(ag_atomic_uint8_t& p) { return p.load(); }
static void ag_atomic_uint8_store(ag_atomic_uint8_t& p, ag_uint8 v) { p.store(v); }
static ag_uint8 ag_atomic_uint8_inc(ag_atomic_uint8_t& p) { return ++p; }
static ag_uint8 ag_atomic_uint8_dec(ag_atomic_uint8_t& p) { return --p; }

extern void AgEmptyObjDraw(AgObj& obj, AgPainter& painter);
extern void AgDrawColorFrame(AgPainter& painter, const AgRect& aera, AgColor color);

/* 锁 */
using ag_lock_t = std::mutex;
extern void AgImpl_LockInit(ag_lock_t& lock);
extern void AgImpl_LockDestroy(ag_lock_t& lock);
extern void AgImpl_Lock(ag_lock_t& lock);
extern void AgImpl_Unlock(ag_lock_t& lock);

/* 内存 */
extern void* AgImpl_MemAlloc(ag_size_t size);
extern void AgImpl_MemFree(void* mem);

}