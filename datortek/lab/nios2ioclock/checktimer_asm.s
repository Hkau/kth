#	.global checktimer
#	.text
#	.align 2

#checktimer:
#       movia r2, -1            # default-värde till r2
#       movia r8, 0x920         # nu pekar r8 på timer-kretsen
#       ldwio r9, 0(r8)         # nu finns status i r2
#       andi  r9, r9, 1         # nu finns Time-Out-biten i r2
#       beq   r9, r0, return    # hoppa om Time-Out-biten är n0ll
#       stwio r0, 0(r8)         # nollställ Time-Out-biten
#       movi  r2, 0             # nollställ r2
#return:
#       ret                     # och så retur med rätt värde i r2: 0 / -1
