.section .vector, "ax"
.align 2
.global vector
vector:
	// set all to null for the moment
	ldr pc, _irq_vector
	_irq_vector: .word 0
	ldr pc, _fiq_vector
	_fiq_vector: .word 0
	ldr pc, _svc_vector
	_svc_vector: .word 0
	ldr pc, _undef_vector
	_undef_vector: .word 0
	ldr pc, _prefetch_vector
	_prefetch_vector: .word 0
	ldr pc, _data_vector
	_data_vector: .word 0