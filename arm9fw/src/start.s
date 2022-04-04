@ Standard boot routine mostly copied from Luma3DS code

.section .crt0, "ax"
.global __crt0__
.align 4
__crt0__:
    @ Disable interrupts and switch to supervisor mode (also clear flags)
    msr cpsr_cxsf, #0xD3

    @ Setup stack (Supervisor)
    ldr sp, =__stack_top__

    @ Disable Cache / MPU
    mrc p15, 0, r0, c1, c0, 0 @ read control register
    bic r0, #(1<<16)    @ Clear bit 16 (DTCM disable)
    bic r0, #(1<<12)    @ Clear bit 12 (Instruction cache disable)
    bic r0, #(1<<2)     @ Clear bit 2 (Data cache disable)
    bic r0, #(1<<0)     @ Clear bit 0 (MPU)
    mcr p15, 0, r0, c1, c0, 0 @ write control register

    @ Cache operations
    mov r4, #0
    mcr p15, 0, r4, c7, c5, 0           @ Flush Instruction cache
    mcr p15, 0, r4, c7, c6, 0           @ Flsuh Data Cache
    mcr p15, 0, r4, c7, c10, 4          @ Wait for write buffer

    @ Give read/write access to all the memory regions
    ldr r5, =0x33333333         @ Chmod 777 equivalence bit
    mcr p15, 0, r5, c5, c0, 2   @ Write data access
    mcr p15, 0, r0, c5, c0, 3   @ write instruction access

    @ Set MPU permissions and cache settings
    ldr r0, =0xFFFF001D @ ffff0000 32k  | bootrom (unprotected part)
    ldr r1, =0xFFF0001B @ fff00000 16k  | dtcm
    ldr r2, =0x01FF801D @ 01ff8000 32k  | itcm
    ldr r3, =0x08000027 @ 08000000 1M   | arm9 mem
    ldr r4, =0x10000029 @ 10000000 2M   | io mem (Arm9 / first 2MB)
    ldr r5, =0x20000035 @ 20000000 128M | fcram
    ldr r6, =0x1FF00027 @ 1FF00000 1M   | dsp / axi wram
    ldr r7, =0x1800002D @ 18000000 8M   | vram (+ 2MB)
    mov r8, #0x29
    mcr p15, 0, r0, c6, c0, 0
    mcr p15, 0, r1, c6, c1, 0
    mcr p15, 0, r2, c6, c2, 0
    mcr p15, 0, r3, c6, c3, 0
    mcr p15, 0, r4, c6, c4, 0
    mcr p15, 0, r5, c6, c5, 0
    mcr p15, 0, r6, c6, c6, 0
    mcr p15, 0, r7, c6, c7, 0
    mcr p15, 0, r8, c3, c0, 0   @ Write bufferable 0, 3, 5
    mcr p15, 0, r8, c2, c0, 0   @ Data cacheable 0, 3, 5
    mcr p15, 0, r8, c2, c0, 1   @ Inst cacheable 0, 3, 5

    @ Set DTCM address and size to the default values
    ldr r1, =0xFFF0000A        @ set DTCM address and size
    mcr p15, 0, r1, c9, c1, 0  @ set the dtcm Region Register

    @ Enable caches / MPU / ITCM
    mrc p15, 0, r0, c1, c0, 0  @ read control register
    orr r0, r0, #(1<<18)       @ - ITCM enable
    orr r0, r0, #(1<<16)       @ - DTCM enable
    orr r0, r0, #(1<<13)       @ - alternate exception vectors enable
    orr r0, r0, #(1<<12)       @ - instruction cache enable
    orr r0, r0, #(1<<2)        @ - data cache enable
    orr r0, r0, #(1<<0)        @ - MPU enable
    mcr p15, 0, r0, c1, c0, 0  @ write control register

    @ Clear BSS
    ldr r0, =__bss_s
    mov r1, #0
    ldr r2, =__bss_e
    sub r2, r0
    bl memset

    @ Setup additionnal sections
    ldr r0, =__vector_s
    ldr r1, =__vector_lma
    ldr r2, =__vector_e
    sub r2, r0
    bl memcpy

    b wrap_main
.pool

wrap_main:
    ldr r0, =__init_array_start
    ldr r1, =__init_array_end
    
globals_init_loop:
    cmp     r0,r1
    it      lt
    ldrlt   r2, [r0], #4
    blxlt   r2
    blt     globals_init_loop
    
    bl arm9fwmain