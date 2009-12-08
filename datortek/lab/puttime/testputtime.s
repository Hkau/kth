# testputtime.s
# Test file to exercise puttime thoroughly
# Written by F Lundevall 2009
# Copyright abandoned. This file is in the public domain.

.macro  SAVREG  # Saves callee-saved registers, and SP
    movia   r8,regstorage
    stw     r16,0(r8)
    stw     r17,4(r8)
    stw     r18,8(r8)
    stw     r19,12(r8)
    stw     r20,16(r8)
    stw     r21,20(r8)
    stw     r22,24(r8)
    stw     r23,28(r8)
    stw     sp,32(r8)
.endm

.macro  CMPREG  # Checks callee-saved regs and SP against stored copies
    movia   r8,regstorage
    movi    r10,16          # Register number to check
    ldw     r9,0(r8)        # Load from stored copy
    bne     r9,r16,1f       # Compare value
    movi    r10,17
    ldw     r9,4(r8)
    bne     r9,r17,1f
    movi    r10,18
    ldw     r9,8(r8)
    bne     r9,r18,1f
    movi    r10,19
    ldw     r9,12(r8)
    bne     r9,r19,1f
    movi    r10,20
    ldw     r9,16(r8)
    bne     r9,r20,1f
    movi    r10,21
    ldw     r9,20(r8)
    bne     r9,r21,1f
    movi    r10,22
    ldw     r9,24(r8)
    bne     r9,r22,1f
    movi    r10,23
    ldw     r9,28(r8)
    bne     r9,r23,1f
    movi    r10,27
    ldw     r9,32(r8)
    bne     r9,sp,1f
    movi    r10,0           # Set r10 to zero to indicate all regs OK
1:
    beq     r10,r0,2f       # If r10 is nonzero, some reg was changed
    movia   r4,badcompare
    mov     r5,r10          # Register number is sent to printf
    movia   r9,printf
    callr   r9
2:
.endm

.macro  RANDR2  # Randomize R2 with Galois-type LFSR
    # Create constant without using buggy movia
    movi    r10,0xd
    slli    r10,r10,28
    addi    r10,r10,1
    # taps for LFSR are: 32 31 29 1
    movia   r4,randomlfsr
    ldw     r8,0(r4)        # Read current value
    srli    r2,r8,1         # Shift right once
    andi    r9,r8,1
    sub     r9,r0,r9
    and     r9,r9,r10
    xor     r2,r2,r9
    stw     r2,0(r4)
.endm
# Note on RANDR2: this pseudo-random number generator uses a
# linear-feedback shift-register algorithm. This gives acceptable
# randomness for our purposes, without using integer multiplication.
# The Nios II ISS does not implement multiply instructions.

.macro  RNDREG  # Stores pseudo-random values in callee-save regs
    RANDR2
    mov             r16,r2
    RANDR2
    mov             r17,r2
    RANDR2
    mov             r18,r2
    RANDR2
    mov             r19,r2
    RANDR2
    mov             r20,r2
    RANDR2
    mov             r21,r2
    RANDR2
    mov             r22,r2
    RANDR2
    mov             r23,r2
.endm

    .data
    .align 2
testmytime: .word 0         # 4 bytes for local time
regstorage: .fill 128,1,0   # enough bytes for registers, and then some
backuptime: .word 0         # backup, in case puttime modifies time
randomlfsr: .word 0xabcdef  # seed for random number generator
badcompare: .asciz "\nerror: register R%d was modified\n"
mytimelost: .asciz "\nerror: mytime variable (in main memory) was modified\n"

    .text
    .align 2
    .global main
main:
    # Someday there may be some code to put here
puttimetestloop:
    SAVREG                  # Save new values
    movia   r4,testmytime   # Parameter to puttime
    movia   r8,puttime
    callr   r8
    CMPREG                  # Compare reg values afterwards
    
    movia   r8,testmytime   # Test if mytime value changed
    ldw     r4,0(r8)
    movia   r5,backuptime
    ldw     r2,0(r5)
    beq     r2,r4,timeok    # If no change, skip printout
    stw     r2,0(r8)        # Restore mytime
    movia   r4,mytimelost
    movia   r9,printf
    callr   r9

timeok:                     # Now update mytime value
    movia   r4,testmytime
    ldw     r8,0(r4)        # Load old value
    slli    r9,r8,4         # Shift left one digit
    addi    r8,r8,1         # Add 1 to unshifted value
    andi    r8,r8,15        # Clear high bits
    add     r9,r9,r8        # Add to shifted value
    stw     r9,0(r4)        # Save new mytime value
    movia   r4,backuptime
    stw     r9,0(r4)        # Save backup copy, too

    RNDREG                  # Change callee-save reg values
                            # (we may do this since main never returns)

    br      puttimetestloop # Go test some more
.end
