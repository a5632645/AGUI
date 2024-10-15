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

#ifndef bool
typedef enum {
    eAgBool_False = 0,
    eAgBool_True,
} ag_bool;

#ifndef true
#define ag_true eAgBool_True
#endif

#ifndef false
#define ag_false eAgBool_False
#endif

#define AGUI_OFFSET_OF(STRUCT, MEMBER) ((size_t) &((STRUCT*)0)->MEMBER)
#define AGUI_CONTAINER_OF(STRUCT, MEMBER, OBJ_ADDR) (STRUCT*)((size_t)(OBJ_ADDR) - AGUI_OFFSET_OF(STRUCT, MEMBER))

#endif