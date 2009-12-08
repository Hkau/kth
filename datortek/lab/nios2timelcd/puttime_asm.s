			.global puttime
			.include "../stack.s"
			.text
			.align 2

puttime:	PUSH	r31				# Add return address to the stack

			PUSH	r16				# We're changing r16, so we need to save its previous value
			ldw		r16, 0(r4)		# Read r4 - save in r16 (safe location)
			PUSH	r16				# Save our r16

			srai	r4, r16, 4		# Shifts MSB towards LSB.
			PUSH	r4
			movi	r4, 0x3A		# :
			PUSH	r4
			srai	r4, r16, 8		# Shifts MSB towards LSB.
			PUSH	r4
			srai	r4, r16, 12		# Shifts MSB towards LSB.
			PUSH	r4

			movia	r16, putchar
			movi	r4, 0x0A		# Newline
			call	putboth

			# print 2 digits
			POP		r4
			call 	hexasc
			mov		r4,r2
			call	putboth			
			POP		r4
			call 	hexasc
			mov		r4,r2
			call	putboth

			# print colon
			POP		r4
			call	putboth

			# print 2 digits
			POP		r4
			call 	hexasc
			mov		r4,r2
			call	putboth
			POP		r4
			call 	hexasc
			mov		r4,r2
			call	putboth
			
			POP 	r16
			POP	 	r31
			ret

putboth:	PUSH	r31				# Add return address to the stack
			PUSH 	r4
			callr 	r16
			POP 	r4
			movia 	r2, lcdput
			callr	r2
			POP		r31
			ret
			
			.end
