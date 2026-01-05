#pragma once
#include "types.h"
#include "drivers/graphics/graphics.h"

#define VGA_COLOR(fg, bg) (((bg) << 4) | ((fg) & 0xF))

GraphicsTextDriver vga_driver_factory();
