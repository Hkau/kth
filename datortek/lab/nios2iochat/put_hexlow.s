	.global put_hexlow
	.text
	.align 2

put_hexlow:	movia r8, 0x9F0 # lcd location
			stwio r4, 0(r8) # write parameter there

			ret				# done :)
