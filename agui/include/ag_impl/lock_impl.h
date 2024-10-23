/*
 * \file lock_impl.h
 * \brief 锁依赖的实现
*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/* 锁 */
void* AgImpl_LockCreate();
void AgImpl_LockDestroy(void* lock);
void AgImpl_Lock(void* lock);
void AgImpl_Unlock(void* lock);

#ifdef __cplusplus
}
#endif
