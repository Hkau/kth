from math import sqrt

def area(a,b,c):
	p = (a + b + c)/2.0

	return sqrt(p*(p-a)*(p-b)*(p-c))

# test! :), borde vara 4*3/2 = 16
print (area(4, 4, sqrt(4**2 + 4**2)))
