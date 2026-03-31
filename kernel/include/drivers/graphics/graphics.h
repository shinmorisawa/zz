#pragma once
#include "types.h"

typedef struct Color {
    u8 r, g, b;
} Color;

typedef struct TextDriver {
    void (*putchar)(struct TextDriver*, u32 x, u32 y, char, Color fg, Color bg);
    void (*clear)(struct TextDriver*);
    void (*scroll)(struct TextDriver*);
    u32 width;
    u32 height;
    void* priv;
} TextDriver;

typedef struct FBDriver {
    void (*setpixel)(struct FBDriver*, u32 x, u32 y, Color);
    u32 width;
    u32 height;
    void* priv;
} FBDriver;
