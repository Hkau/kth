#!/usr/bin/env python
import sys
def modexp(a, e, n):
	if e == 0:
		return 1
	sub = modexp(a, e//2,  n)
	sub *= sub
	if e & 1 != 0:
		sub *= a
	return sub % n

for line in sys.stdin:
	if line == "":
		continue
	vals = [int(a) for a in line.split()]
	if vals[2] == 1:
		print 0
		continue
	print modexp(vals[0], vals[1], vals[2])

