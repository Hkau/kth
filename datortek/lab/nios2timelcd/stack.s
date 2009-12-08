.macro PUSH reg
  		subi    sp, sp, 4
		stw     \reg, 0(sp)
.endm

.macro POP reg
		ldw     \reg, 0(sp)
		addi    sp, sp, 4
.endm
