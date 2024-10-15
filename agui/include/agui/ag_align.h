#pragma once

typedef enum {
    eAgAlign_XLeft = 0b01,
    eAgAlign_XCenter = 0b10,
    eAgAlign_XRight = 0b11,

    eAgAlign_YTop = 0b0100,
    eAgAlign_YCenter = 0b1000,
    eAgAlign_YBottom = 0b1100,

    eAgAlign_Center = eAgAlign_XCenter | eAgAlign_YCenter,
    eAgAlign_None = 0,
} AgAlignEnum;

#define AGUI_X_ALIGN(x) ((x) & 0b11)
#define AGUI_Y_ALIGN(y) (((y) & 0b1100))