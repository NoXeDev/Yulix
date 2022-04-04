OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(__crt0__)

MEMORY
{
    PRIVMEM     (RWX)   :   ORIGIN = 0x08080000, LENGTH = 64K
    INTVECTOR   (RWX)   :   ORIGIN = 0x08000000, LENGTH = 64
}

SECTIONS
{
    PROVIDE(__start__           = 0x08080000);
    PROVIDE(__stack_top__       = 0x08100000);

    . = __start__;

    .crt0   :   ALIGN(16)
    {
        __crt0_start = ABSOLUTE(.);
        KEEP(*(.crt0))
        . = ALIGN(16);
    } >PRIVMEM

    /* Vector section (IRQ, Databort, etc...)*/
    .vector :   ALIGN(16)
    {
        __vector_lma = LOADADDR(.vector);
        __vector_s = ABSOLUTE(.);
        KEEP(*(.vector))
        . = ALIGN(16);
        __vector_e = ABSOLUTE(.);
    } >INTVECTOR AT>PRIVMEM

    .text : ALIGN(16)
    {
        __text_lma = LOADADDR(.text);
        __text_s = ABSOLUTE(.);
        *(.text*)
        . = ALIGN(16);
        __text_e = ABSOLUTE(.);
    } >PRIVMEM

    .data : ALIGN(16)
    {
        __data_lma = LOADADDR(.data);
        __data_s = ABSOLUTE(.);

        __init_array_start = .;
        *(.init_array)
        *(.init_array.*)
        __init_array_end = .;

        *(.data*)
        *(.rodata*)

        . = ALIGN(16);
        __data_e = ABSOLUTE(.);
    } >PRIVMEM

    .bss (NOLOAD) : ALIGN(16)
    {
        __bss_s = ABSOLUTE(.);
        *(.bss*)
        . = ALIGN(16);
        __bss_e = ABSOLUTE(.);
    } >PRIVMEM
}