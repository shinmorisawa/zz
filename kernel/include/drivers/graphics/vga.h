#pragma once
#include "types.h"
#include "drivers/graphics/graphics.h"

#define VGA_BASE 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

TextDriver* vga_driver_factory();
