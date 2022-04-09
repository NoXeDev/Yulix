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
        KEEP(*(.init))
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
        *(.text)
        *(.text.*)
        *(.glue_7)
        *(.glue_7t)
        *(.stub)
        *(.gnu.warning)
        *(.gnu.linkonce.t*)
        KEEP( *(.fini) )
        . = ALIGN(16);
    } >PRIVMEM

    .rodata :
    {
        *(.rodata)
        *(.roda)
        *(.rodata.*)
        *all.rodata*(*)
        *(.gnu.linkonce.r*)
        SORT(CONSTRUCTORS)
        . = ALIGN(8);
    } >PRIVMEM

    .preinit_array :
    {
        PROVIDE (__preinit_array_start = .);
        KEEP (*(.preinit_array))
        PROVIDE (__preinit_array_end = .);
    } >PRIVMEM

    .init_array ALIGN(4) :
    {
        PROVIDE (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array))
        PROVIDE (__init_array_end = .);
    } >PRIVMEM

    .fini_array ALIGN(4) :
    {
        PROVIDE (__fini_array_start = .);
        KEEP (*(.fini_array))
        KEEP (*(SORT(.fini_array.*)))
        PROVIDE (__fini_array_end = .);
    } >PRIVMEM

    .ctors ALIGN(4) :
    {
        KEEP (*crtbegin.o(.ctors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*(.ctors))
        . = ALIGN(4);
    } >PRIVMEM

    .dtors ALIGN(4) :
    {
        KEEP (*crtbegin.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*(.dtors))
        . = ALIGN(4);
    } >PRIVMEM

    .ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) __exidx_start = ABSOLUTE(.);} >PRIVMEM
    ARM.exidx : { *(.ARM.exidx* .gnu.linkonce.armexidx.*)  __exidx_end = ABSOLUTE(.);} >PRIVMEM

    .data :
    {
        *(.data)
        *(.data.*)
        CONSTRUCTORS
        . = ALIGN(32);
    } >PRIVMEM

    .bss (NOLOAD) :
    {
        . = ALIGN(32);
        PROVIDE (__bss_s = ABSOLUTE(.));
        *(.dynbss)
        *(.bss)
        *(.bss.*)
        *(.gnu.linkonce.b*)
        *(COMMON)
        . = ALIGN(32);
        PROVIDE (__bss_e = ABSOLUTE(.));
    } >PRIVMEM :NONE

    __end__ = ABSOLUTE(.) ;
}