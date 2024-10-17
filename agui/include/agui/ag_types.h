#pragma once
#include <stdint.h>
#include <stddef.h>

typedef uint32_t ag_uint32;
typedef int32_t ag_int32;

typedef uint16_t ag_uint16;
typedef int16_t ag_int16;

typedef uint8_t ag_uint8;
typedef int8_t ag_int8;

typedef float ag_float;
typedef double ag_double;

typedef size_t ag_size_t;

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

#define AGUI_OFFSET_OF(STRUCT, MEMBER) ((size_t) &((STRUCT*)0)->MEMBER)
#define AGUI_CONTAINER_OF(STRUCT, MEMBER, OBJ_ADDR) (STRUCT*)((size_t)(OBJ_ADDR) - AGUI_OFFSET_OF(STRUCT, MEMBER))
#define AGUI_MIN(a, b) ((a) < (b) ? (a) : (b))
#define AGUI_MAX(a, b) ((a) > (b) ? (a) : (b))
