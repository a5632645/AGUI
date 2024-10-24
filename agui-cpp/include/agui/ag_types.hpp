#pragma once
#include <stdint.h>
#include <stddef.h>

namespace agui {

typedef uint32_t ag_uint32;
typedef int32_t ag_int32;

typedef uint16_t ag_uint16;
typedef int16_t ag_int16;

typedef uint8_t ag_uint8;
typedef int8_t ag_int8;

typedef float ag_float;
typedef double ag_double;

typedef size_t ag_size_t;

#ifdef __cplusplus

using ag_bool = bool;
static constexpr ag_bool ag_true = true;
static constexpr ag_bool ag_false = false;

#else

#ifndef bool
typedef uint8_t ag_bool;
#else
typedef bool ag_bool;
#endif

#ifndef true
#define ag_true 1
#else
#define ag_true true
#endif

#ifndef false
#define ag_false 0
#else
#define ag_false false
#endif

#endif

#define AGUI_OFFSET_OF(STRUCT, MEMBER) ((size_t) &((STRUCT*)0)->MEMBER)
#define AGUI_CONTAINER_OF(STRUCT, MEMBER, OBJ_ADDR) (STRUCT*)((size_t)(OBJ_ADDR) - AGUI_OFFSET_OF(STRUCT, MEMBER))
#define AGUI_MIN(a, b) ((a) < (b) ? (a) : (b))
#define AGUI_MAX(a, b) ((a) > (b) ? (a) : (b))

template<class T>
struct NullablePtr {
    T* ptr{};
    constexpr NullablePtr() = default;
    constexpr NullablePtr(T* ptr) : ptr(ptr) {}

    constexpr operator T*() const { return ptr; }
    constexpr T& operator*() const { return *ptr; }
    constexpr T* operator->() const { return ptr; }
    constexpr bool operator!() const { return !ptr; }

    template<class E>
    constexpr bool operator==(E* other) const { return ptr == other; }

    template<class F>
    void CallIf(F&& f) const { if (ptr) f(*ptr); }
};

template<>
struct NullablePtr<void> {
    void* ptr{};
    constexpr NullablePtr() = default;
    constexpr NullablePtr(void* ptr) {}

    constexpr operator void*() const { return ptr; }
    constexpr bool operator!() const { return !ptr; }

    template<class E>
    constexpr bool operator==(E* other) const { return ptr == other; }

    template<class F>
    void CallIf(F&& f) const { if (ptr) f(ptr); }
};

}
