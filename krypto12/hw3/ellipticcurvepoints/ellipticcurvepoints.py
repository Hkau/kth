#!/usr/bin/env python
import sys

def singular(p, a, b):
	# discriminant for y^2 = x^3+ax+b is given
	# by: -4a^3-27b^2
	discriminant = (-4*a**3-27*b**2) % p

	if discriminant == 0:
		return 1 # singular
	else:
		return 0 # smooth

def ecp(p, a, b):
	return (singular(p,a,b,), -1)

# just i/o
for line in sys.stdin:
	if line == "":
		continue
	vals = [int(a) for a in line.split()] # read ALL the ints
	(s, n) = ecp(vals[0], vals[1], vals[2])
	print s, n

