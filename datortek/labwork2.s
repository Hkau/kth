        # Lab program for Nios-II IDE tutorial
        # Created 2007-10-24 by F Lundevall
        # Modified 2008-10-08 by F Lundevall
        # Modified 2009-07-21 by F Lundevall
        # Copyright abandoned. This file is in the public domain.
        
        .global main            # makes label "main" globally known

        .text                   # Instructions follow
		.align  2				# Align instructions to 4-byte words

main:   movi    r16,0x41        # Load the hexadecimal value 41
                                # to register r16

loop:   mov     r4,r16          # Copy to r4 from r16

        nop                     # (later changed to call hexasc)
        nop                     # (later changed to mov r4,r2)

        movia   r8,putchar      # copy subroutine address to a register
        callr   r8              # call subroutine via register

        addi    r16, r16,1      # Add 1 to register r16
        andi    r16, r16, 0x7f  # mask with 7 bits
        ori     r16, r16, 0x20  # set a bit to avoid control chars

        br      loop            # Branch to loop

        .end                    # The assembler will stop reading here
        foo bar bletch          # comes after .end - ignored
