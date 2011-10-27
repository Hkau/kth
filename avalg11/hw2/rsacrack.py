#!/usr/bin/env python3
from math import sqrt

n = 5192042249411
e = 3419183406883

# n = p * q where p and q are primes

for i in range(3, int(sqrt(n))+1, 2):
	if n % i == 0:
		p = i
		q = n // i
		print('p =', p, 'q =', q)
		break

p = q
