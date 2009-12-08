	.global out_char
	.global in_charx
	.text
	.align 2

out_char:	andi	r4, r4, 0xFF	# mask out byte to be sent
			movia	r2, 0x868		# serial status for transmit

startloop:	ldwio	r3, 0(r2)		# read status field
			andi	r3, r3, 64		# mask 7th bit

			beq		r3, r0, startloop	# go back to start of loop until txready bit is set

			movia	r2, 0x864		# send field
			stwio	r4,	0(r2)		# send that byte

			ret

in_charx:	movia	r2, 0x868		# serial status for receive
			ldwio	r2, 0(r2)		# read status
			andi	r2, r2, 128		# mask 8th bit
			beq		r2, r0, return  # if not ready, abort

			movia	r2, 0x860		# location of receive
			ldwio	r2, 0(r2)		# read our received byte
			andi	r2, r2, 0xFF	# mask 8 lsb
			ret

return:		movi	r2, -1
			ret
