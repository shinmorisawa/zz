#include "types.h"
#include "utils.h"
#include "interrupt.h"
#include "handlers.h"

static IDTEntry idt[256];
static IDTPointer idt_ptr;

extern void isr32();
extern void isr14();
extern void isr0();
extern void isr1();

static void pic_remap(void) {
    u8 a1 = inb(0x21);
    u8 a2 = inb(0xA1);

    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    outb(0x21, a1);
    outb(0xA1, a2);
}

void idt_add_entry(int n, void (*handler)()) {
    u64 addr = (u64)handler;
    idt[n].offset_low  = addr & 0xFFFF;
    idt[n].segment     = 0x08;
    idt[n].ist         = 0;
    idt[n].type        = 0x8E;
    idt[n].offset_mid  = (addr >> 16) & 0xFFFF;
    idt[n].offset_high = (addr >> 32) & 0xFFFFFFFF;
    idt[n].reserved    = 0;
}

void pit_init(void) {
    outb(0x43, 0x36);

    outb(0x40, PIT_DIVIDER & 0xFF);
    outb(0x40, PIT_DIVIDER >> 8);
}

void idt_init(void) {
    pic_remap();
    pit_init();
    idt_add_entry(0x20, isr32);
    idt_add_entry(0x0, isr0);
    idt_add_entry(0x1, isr1);
    idt_add_entry(0xE, isr14);

    idt_ptr.limit = sizeof(idt) - 1;
    idt_ptr.base  = (u64)&idt;

    asm volatile ("lidt %0" : : "m"(idt_ptr));
    outb(0x21, 0xFE);
    asm volatile ("sti" : : :);
}

void pic_eoi(void) {
    outb(0x20, 0x20);
}
