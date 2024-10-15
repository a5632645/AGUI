#include "agui/ag_obj.h"

typedef struct {
    AgObj obj;
    const char* text;
    AgColor color;
    uint32_t font_size;
} TextView;

void TextView_Init(TextView* tv);