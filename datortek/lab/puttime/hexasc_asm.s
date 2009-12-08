# Name: The subroutine must be called hexasc.
# Input parameters: Only one, in register r4. The 4 least significant bits in register r4 specify a number, from 0 through 15. The values of all other bits in the input must be ignored.
# Return value: Only one, returned in register r2. The 7 least significant bits in register r2 must be an ASCII code as described below. All other bits in the output must be zero.
# Required action: Input values 0 through 9 must be converted to the ASCII codes for the digits '0' through '9', respectively. Input values 10 through 15 must be converted to the ASCII codes for the letters 'A' through 'F', respectively.
# Side effects: The values in registers r2 through r15 may be changed. All other registers must have unchanged values when the subroutine returns.

			.global hexasc
			.text
			.align 2
			
hexasc:		andi	r2, r4, 0xF			# Mask 4 bits (we don't care about the rest)
			addi	r2, r2, 0x30		# 0000 => ascii 0 (= 0x30)
			movi	r15, 0x3A			# Save 0x3A to r15
			bge		r2, r15, letter		# If 0x40 or bigger, it's a character
			ret
			
letter:		addi	r2, r2, 7		# Add 7 to avoid characters
			ret
			
			.end
