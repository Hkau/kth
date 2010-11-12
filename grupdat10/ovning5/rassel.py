# coding: utf8
raffel = list(range(52))

l = raffel

iter = 0
while True:
	tmp = []
	for i in range(len(l)//2):
		tmp += [l[i], l[len(l)//2+i]];
	l = tmp
	print(l)
	iter += 1
	if raffel == l:
		break

print("Raffelcykeln tog", iter, "gånger")

