#pragma once
#include "agui/ag_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct __AgObj;

typedef struct {
    ag_bool handled;
    ag_uint8 class_;
    ag_uint8 id;
    struct __AgObj* sender;
} AgEvent;


#ifdef __cplusplus
}
#endif
