OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(__crt0__)

MEMORY
{
    AXIRAM     (RWX)   :   ORIGIN = 0x1FF80000, LENGTH = 64K
    INTVECTOR   (RWX)   :   ORIGIN = 0x1FFFFFA0, LENGTH = 512
}

SECTIONS
{
    PROVIDE(__start__           = 0x1FF80000);
    PROVIDE(__stack_top__       = 0x1FFFE000);

    . = __start__;

    .crt0   :   ALIGN(16)
    {
        __crt0_start = ABSOLUTE(.);
        KEEP(*(.crt0))
        . = ALIGN(16);
    } >AXIRAM
    
    .vector :   ALIGN(16)
    {
        __vector_lma = LOADADDR(.vector);
        __vector_s = ABSOLUTE(.);
        KEEP(*(.vector))
        . = ALIGN(16);
        __vector_e = ABSOLUTE(.);
    } >INTVECTOR AT>AXIRAM

    .text : ALIGN(16)
    {
        __text_lma = LOADADDR(.text);
        __text_s = ABSOLUTE(.);
        *(.text*)
        . = ALIGN(16);
        __text_e = ABSOLUTE(.);
    } >AXIRAM

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
    } >AXIRAM

    .bss (NOLOAD) : ALIGN(16)
    {
        __bss_s = ABSOLUTE(.);
        *(.bss*)
        . = ALIGN(16);
        __bss_e = ABSOLUTE(.);
    } >AXIRAM
}