void write_char(char a, char color, int index) {
    volatile char* vga = (volatile char*)0xB8000;
    vga[index * 2] = a;
    vga[index * 2 + 1] = color;
}

void write_string(char* string, char color, int y, int x) {
    int index = x + (y * 80);
    while (*string != '\0') {
        write_char(*string, color, index);
        string++;
        index++;
    }
}

void kstart(void) {
    write_string("zz kernel...", 0x0F, 0, 0);
    write_string("booted!!!!", 0x0F, 1, 0);

    for (;;) {}
}
