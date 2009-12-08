	.global check_key3
	.text
	.align 2

check_key3:	movi	r4, 0x840			# de2_pio_keys4_keys
			ldwio	r2, 0(r4)			# de2_pio_keys4 keys' location
			andi 	r2, r2, 0x08		# maska ut key3
			beq 	r2, r0, return		# om key var noll, returnera noll
			movi 	r2, -1				# -1 = alla bittar satta :)

return:		ret
