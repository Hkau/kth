#!/usr/bin/env python3
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

n = 8902240814
print('n =', n)
(a, b, gcd) = egcd(n, n+1)

assert gcd == 1

# ua + vb = gcd(a, b) = 1
# ua = 1 - vb mod(b) = ua = 1 mod(b)
# vb = 1 - ua mod(a) = vb = 1 mod(a)

# ua*r2 + vb*r1 mod(b) <=> r2 mod(b)
# ua*r2 + vb*r1 mod(a) <=> r1 mod(a)

r1 = 123456789
r2 = 987654321
x = a*n*r1 + b*(n+1)*r2

print(x)
print(x % n)
print(x % (n+1))
