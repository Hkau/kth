	.global snaptime
	.text
	.align 2

# Useful definitions to copy into your init_timer_asm.s file
.equ    timer1_status,        (0x920)         # status
.equ    timer1_control,       (0x924)         # control
.equ    timer1_period_low,    (0x928)         # period low
.equ    timer1_period_high,   (0x92C)         # period high
.equ    timer1_snap_low,      (0x930)         # snapshot low
.equ    timer1_snap_high,     (0x934)         # snapshot high

snaptime:	movia 	r4, timer1_snap_low
			stwio 	r0, 0(r4)					# write something to 0x930 to take a snapshot
			ldwio 	r2, 4(r4)					# read snapshot msb
			slli 	r2, r2, 16						# shift left
			ldwio 	r4, 0(r4)					# read snapshot lsb
			andi 	r4, r4, 0xFFFF				# make sure no junk bits get in the way (mask 16 bits)
			or	 	r2, r2, r4					# put the msb/lsb together

			ret
