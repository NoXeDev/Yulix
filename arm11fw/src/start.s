.section .crt0, "ax"
.align 4
.global __crt0__
__crt0__:
    @ Disable interrupts and switch to supervisor mode
    cpsid aif, #0x13

    @ Set the control register to reset default: everything disabled
    ldr r0, =0x54078
    mcr p15, 0, r0, c1, c0, 0

    @ Set the auxiliary control register to reset default.
    @ Enables instruction folding, static branch prediction,
    @ dynamic branch prediction, and return stack.
    mov r0, #0xF
    mcr p15, 0, r0, c1, c0, 1

    @ Invalidate both caches, flush the prefetch buffer then DSB
    mov r0, #0
    mcr p15, 0, r0, c7, c5, 4
    mcr p15, 0, r0, c7, c7, 0
    mcr p15, 0, r0, c7, c10, 4

    ldr sp, =__stack_top__
    mov fp, #0

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
    
    mrc p15, 0, r12, c0, c0, 5
    ands r12, r12, #3
    beq wrap_main

    @ If arm11 other cores was previously enabled, make sure they will loop forever
    b proc_hang
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
    
    bl arm11fw_main

proc_hang: 
    wfi
    b proc_hang