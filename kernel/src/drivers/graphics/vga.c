#include "types.h"
#include "drivers/graphics/vga.h"
#include "drivers/graphics/graphics.h"

static u8 vga_color(Color color) {
    // grays
    if (color.r > 0xB0 && color.g > 0xB0 && color.b > 0xB0) return 0xF;
    if (color.r > 0x80 && color.g > 0x80 && color.b > 0x80) return 0x7;
    if (color.r > 0x40 && color.g > 0x40 && color.b > 0x40) return 0x8;
    // red
    if (color.r > 0xB0 && color.g < 0x40 && color.b < 0x40) return 0xC;
    if (color.r > 0x80 && color.g < 0x40 && color.b < 0x40) return 0x4;
    // green
    if (color.r < 0x40 && color.g > 0xB0 && color.b < 0x40) return 0xA;
    if (color.r < 0x40 && color.g > 0x80 && color.b < 0x40) return 0x2;
    // blue
    if (color.r < 0x40 && color.g < 0x40 && color.b > 0xB0) return 0x9;
    if (color.r < 0x40 && color.g < 0x40 && color.b > 0x80) return 0x1;

    return 0x0;
}

static void vga_putchar(TextDriver* driver, u32 x, u32 y, char c, Color fg, Color bg) {
    if (x >= VGA_WIDTH)
        return;
    if (y >= VGA_HEIGHT)
        return;

    u8 fg_vga = vga_color(fg);
    u8 bg_vga = vga_color(bg);

    volatile u8* vga = (volatile u8*)VGA_BASE; 
    vga[(VGA_WIDTH * y + x) * 2] = c;
    vga[(VGA_WIDTH * y + x) * 2 + 1] = bg_vga << 4 | fg_vga;
}

static void vga_clear(TextDriver* driver) {
    for (u32 y = 0; y < driver->height; y++) {
        for (u32 x = 0; x < driver->width; x++) {
            volatile u8* vga = (volatile u8*)VGA_BASE;
            vga[(VGA_WIDTH * y + x) * 2] = ' ';
            vga[(VGA_WIDTH * y + x) * 2 + 1] = 0x07;
        }
    }
}

static TextDriver vga_driver = {
    .putchar = vga_putchar,
    .clear = vga_clear,
    .width = VGA_WIDTH,
    .height = VGA_HEIGHT,
};

TextDriver* vga_driver_factory(void) {
    return &vga_driver;
}
