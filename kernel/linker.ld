ENTRY(_start)

SECTIONS {
    . = 0x200000;
    .text : {
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    .bss : {
        *(COMMON)
        *(.bss*)
    }

    kernel_end = .;

    . = ALIGN(16);
    kernel_stack_bottom = .;
    . += 32K;
    kernel_stack_top = .;
    . += 4K;
}
