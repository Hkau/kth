			.global delay
			.text
			.align 2
			.equ    delaycount,     15385
			
delay:		mov		r2, r4 				# r4 must be unchanged
			muli	r2,r2, delaycount	# Multiply by constant, synched to hardware

count:		subi	r2, r2, 1			# Lower counter by one
			bgt		r2, r0, count		# Go to count if r2 >= 0
			
			.end
