#include "ag_impl/lock_impl.hpp"
#include "ag_impl/mem_impl.hpp"
#include <stdlib.h>
#include <semaphore.h>
#include <stdatomic.h>

namespace agui {

// ---------------------------------------- mem ----------------------------------------
void* AgImpl_MemAlloc(ag_size_t size) {
    return malloc(size);
}

void AgImpl_MemFree(void* mem) {
    free(mem);
}

// ---------------------------------------- lock ----------------------------------------
void* AgImpl_LockCreate() {
    void* lock = malloc(sizeof(sem_t));
    sem_init((sem_t*)lock, 0, 1);
    return lock;
}

void AgImpl_LockDestroy(void* lock) {
    sem_destroy((sem_t*)lock);
    free(lock);
}

void AgImpl_Lock(void* lock) {
    sem_wait((sem_t*)lock);
}

void AgImpl_Unlock(void* lock) {
    sem_post((sem_t*)lock);
}

}
