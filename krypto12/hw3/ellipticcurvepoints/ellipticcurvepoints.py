#!/usr/bin/env python

def ecp(p, a, b):
	return (-1, -1)

# just i/o
for line in sys.stdin:
	if line == "":
		continue
	vals = [int(a) for a in line.split()] # read ALL the ints
	(s, n) = ecp(vals[0], vals[1], vals[2])
	print s, n

