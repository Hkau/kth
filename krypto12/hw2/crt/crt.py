#!/usr/bin/env python
# An implementation of:
# http://en.wikipedia.org/wiki/Chinese_Remainder_Theorem#
#	A_constructive_algorithm_to_find_the_solution
import sys

#ua + vb = gcd(a,b)
def egcd(a, b):
    u, u1 = 1, 0
    v, v1 = 0, 1
    g, g1 = a, b
    while g1:
        q = g // g1
        u, u1 = u1, u - q * u1
        v, v1 = v1, v - q * v1
        g, g1 = g1, g - q * g1
    return u, v, g

def crt(k, num, mod):
	# Product of all moduli
	N = 1
	for m in mod:
		N *= m

	# generate ei parts
	e = []
	for a in mod:
		Ni = N//a # Ni and N are coprime
		(ri, si, gcd) = egcd(a, Ni)
		assert gcd == 1 # unless something went wrong, they should be coprime
		e.append(si*Ni)

	# sum up pairwise products to generate final x
	x = 0
	for i in range(k):
		x += num[i]*e[i]
	return x % N # positive result mod N

for line in sys.stdin:
	if line == "":
		continue
	vals = [int(a) for a in line.split()]
	k = vals[0] # k congruences
	mod = []
	for i in range(k): # k coprime moduli
		mod.append(vals[i+1])
	num = []
	for i in range(k): # k integers
		num.append(vals[i+k+1])
	print crt(k, num, mod)

