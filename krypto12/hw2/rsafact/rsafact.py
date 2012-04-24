#!/usr/bin/env python
import sys
import random

def modexp(a, e, n):
	if e == 0:
		return 1
	sub = modexp(a, e//2,  n) # modexp for halv the e, truncated
	sub *= sub # square it
	if e & 1 != 0: # if e was odd, multiply in another a, fixes truncation part
		sub *= a
	return sub % n

def gcd(a, b):
	if b == 0:
		return a
	return gcd(b, a % b)
	

def rsafact(n, e, d):
	r = e * d - 1
	s = 0
	while r % 2 == 0:
		s += 1
		r //= 2

	# try to factor until success
	while True:
		w = random.randint(1, n-1)
		x = gcd(w, n)
		if x != 1: # x multiple of n, LUCKY!
			return x

		v = modexp(w, r, n)
		if v % n == 1: # 1 mod n => failure, try again
			continue

		while v % n != 1:
			v0 = v
			v = modexp(v, 2, n)
	
		if v0 % n == n-1: # -1 mod n => failure, try again
			continue
	
		x = gcd(v0 + 1, n)
		return x
	

for line in sys.stdin:
	if line == "":
		continue
	vals = [int(a) for a in line.split()] # read ALL the ints
	if vals[2] == 1:
		print 0
		continue
	p = rsafact(vals[0], vals[1], vals[2])
	q = vals[0] // p
	# smallest factor first
	if q < p:
		print q, p
	else:
		print p, q

