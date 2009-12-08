	.equ timer_timeout, (10)		# timer timeout in milliseconds
	.equ timer_address, (0x920)		# memory mapped timer's location

	.global init_timer
	.text
	.align 2

init_timer:		movia r8, timer_address

				# beräkna total timeout
				movia r9, 50000
				muli r9, r9, timer_timeout
				subi r9, r9, 1

				# move r9 to periodl and periodh
				stwio r9, 8(r8)
				srli r9, r9, 16
				stwio r9, 12(r8)

				# set run bit (why is this not 0x02?)
				movi r9, 0x06
				stwio r9, 4(r8)

				stwio r0, 0(r8)

				# done
				ret
