#pragma once
#include "types.h"

typedef enum VGAColor {
    BLACK = 0x0,
    BLUE = 0x1,
    GREEN = 0x2,
    CYAN = 0x3,
    RED = 0x4,
    MAGENTA = 0x5,
    BROWN = 0x6,
    WHITE = 0x7,
    GRAY = 0x8,
    LIGHT_BLUE = 0x9,
    LIGHT_GREEN = 0xA,
    LIGHT_CYAN = 0xB,
    LIGHT_RED = 0xC,
    LIGHT_MAGENTA = 0xD,
    YELLOW = 0xE,
    BRIGHT_WHITE = 0xF
} VGAColor;

typedef struct GraphicsTextDriver {
    void (*putchar)(char, VGAColor, int);
    void (*puts)(const char*, VGAColor, int, int);
} GraphicsTextDriver;

typedef struct FBDriver {

} FBDriver;
