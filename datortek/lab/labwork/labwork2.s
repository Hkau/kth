        # Lab program for Nios-II IDE tutorial
        # Created 2007-10-24 by F Lundevall
        # Modified 2008-10-08 by F Lundevall
        # Modified 2009-07-21 by F Lundevall
        # Copyright abandoned. This file is in the public domain.
        
     		.global main            # makes label "main" globally known

     		.text                   # Instructions follow
     		.align  2				# Align instructions to 4-byte words (2^2 = 4)

main:   	movi    r16,0x41        # Load the hexadecimal value 41 (A)
                               		# to register r16

loop:   	mov     r4,r16          # Copy to r4 from r16 (putchar reads from r4)

     		call 	hexasc			# Call our subrt
     		mov 	r4,r2			# Move the result from r2 to r4

     		movia   r8,putchar      # copy subroutine address to a register
     		callr   r8              # call subroutine via register

    	    addi    r16, r16,1      # Add 1 to register r16 (eg. 0x41 -> 0x42)
    	    andi    r16, r16, 0x7f  # mask with 7 bits (clear first bit, must never be 1 (negative))
     		ori     r16, r16, 0x20  # set a bit to avoid control chars (b6 may not be 0)

     		br      loop            # Branch to loop


			.end
     		foo bar bletch          	# comes after .end - ignored
