OUTPUT_FORMAT("binary")
ENTRY(start)

SECTIONS {
    . = 0x7C00;
    .mbr : {
        *(.mbr)
        . = 0x7C00 + 510;
        SHORT(0xAA55);
    }

    . = 0x8000;
    .stage1 : {
        *(.stage1)
    }

    . = 0xC000;
    .stage2 : {
        *(.stage2)
        . = ALIGN(512);
    }

    . = ALIGN(512);
}
