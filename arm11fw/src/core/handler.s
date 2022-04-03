.section .vector, "ax"
.align 2
.global vector
vector:
	ldr pc, _irq_vector
	ldr pc, _fiq_vector
	ldr pc, _swi_vector
	ldr pc, _undef_vector
	ldr pc, _instruction_vector
	ldr pc, _data_vector

#define MPCORE_PMR   (0x17E00000)
#define IRQ_SPURIOUS (1023)
#define IRQ_COUNT    (128)

_irq_vector:
    sub lr, lr, #4
    srsfd sp!, #0x13
    cps #0x13
    push {r0-r3, r12, lr}

    1:
    ldr lr, =MPCORE_PMR
    ldr r0, [lr, #0x10C]

    ldr r1, =IRQ_SPURIOUS
    cmp r0, r1
    beq 3f

    cmp r0, #IRQ_COUNT
    bhs 2f

    ldr r12, =GIC_handlerArray
    ldr r12, [r12, r0, lsl #2]
    cmp r12, #0
    beq 2f

    push {r0, r12}
    blx r12
    pop {r0, r12}

    2:
    ldr lr, =MPCORE_PMR
    str r0, [lr, #0x110]       @ End of interrupt
    b 1b                       @ Check for any other pending interrupts

    3:
    pop {r0-r3, r12, lr}       @ Restore registers
    rfeia sp!                  @ Return From Exception

_fiq_vector:
    .word 0

_swi_vector:
    .word 0

_undef_vector:
    .word 0

_instruction_vector:
    .word 0

_data_vector:
    .word 0
