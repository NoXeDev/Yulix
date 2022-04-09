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

    .text :
    {
        KEEP( *(.text.start) )
        KEEP( *(.init) )

        /* .text */
        *(.text)
        *(.text.*)
        *(.glue_7)
        *(.glue_7t)
        *(.stub)
        *(.gnu.warning)
        *(.gnu.linkonce.t*)

        /* .fini */
        KEEP( *(.fini) )
        . = ALIGN(32);
    } >AXIRAM

    .rodata :
    {
        *(.rodata)
        *(.roda)
        *(.rodata.*)
        *all.rodata*(*)
        *(.gnu.linkonce.r*)
        SORT(CONSTRUCTORS)
        . = ALIGN(8);
    } >AXIRAM

    .preinit_array :
    {
        PROVIDE (__preinit_array_start = .);
        KEEP (*(.preinit_array))
        PROVIDE (__preinit_array_end = .);
    } >AXIRAM

    .init_array :
    {
        PROVIDE (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array))
        PROVIDE (__init_array_end = .);
    } >AXIRAM

    .fini_array :
    {
        PROVIDE (__fini_array_start = .);
        KEEP (*(.fini_array))
        KEEP (*(SORT(.fini_array.*)))
        PROVIDE (__fini_array_end = .);
    } >AXIRAM

    .ctors :
    {
        KEEP (*crtbegin.o(.ctors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*(.ctors))
        . = ALIGN(4);
    } >AXIRAM

    .dtors :
    {
        KEEP (*crtbegin.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*(.dtors))
        . = ALIGN(4);
    } >AXIRAM

    .ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) __exidx_start = .;}
    ARM.exidx : { *(.ARM.exidx* .gnu.linkonce.armexidx.*)  __exidx_end = .;}

    .data :
    {
        *(.data)
        *(.data.*)
        *(.gnu.linkonce.d*)
        CONSTRUCTORS
        . = ALIGN(32);
    } >AXIRAM

    .bss (NOLOAD) :
    {
        . = ALIGN(32);
        PROVIDE (__bss_s = ABSOLUTE(.));
        *(.dynbss)
        *(.bss)
        *(.bss.*)
        *(.gnu.linkonce.b*)
        *(COMMON)
        . = ALIGN(8);
        PROVIDE (__bss_e = ABSOLUTE(.));
    } :NONE

    PROVIDE (__end__ = ABSOLUTE(.));
}