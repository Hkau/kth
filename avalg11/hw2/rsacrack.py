#!/usr/bin/env python3
from math import sqrt

n = 5192042249411
e = 3419183406883

print('n =', n)
print('e =', e)

# n = p * q where p and q are primes
# n is odd in this case, so our method is ok
for i in range(3, int(sqrt(n))+1, 2):
	if n % i == 0:
		p = i
		q = n // i
		print('p =', p, 'q =', q)
		break

tot = (p-1)*(q-1)
print('tot(n) =', tot)

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

(a, b, gcd) = egcd(e, tot)

assert gcd == 1

d = a

print('d =', d)

# exponentiation by squaring
def modpow(x, n, mod):
	if n == 0:
		return 1
	root = modpow(x, n//2, mod)
	if n & 1:
		return (x*root*root)%mod
	return (root*root)%mod

c = 8902240814
plain = modpow(c,d,n)
print("c^d mod n", "( c =", c, "d =", d, 'n =', n, ') = ', plain) 
assert(modpow(plain, e, n) == c)
print("m^e mod n", "( m =", plain, "e =", e, 'n =', n, ') = ', c) 

# Assert that our cryptotext is valid

