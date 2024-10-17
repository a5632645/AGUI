#pragma once
#include "agui/ag_types.h"

struct __AgObj;

typedef struct {
    ag_bool handled;
    ag_uint16 type;
    struct __AgObj* sender;
    void* ptr;
} AgEvent;