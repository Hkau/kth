def summa(v):
	n = 0
	for i in range(len(v)):
		n += v[i]
	return n

#alt.

def summa(v):
	return summa_rekursiv(v, 0, len(v)-1)

def summa_rekursiv(v, first, last):
	if first == last: # om det bara ska summeras ett element (first = sista index)
		return v[first]
	else:
		return v[first] + summa_rekursiv(v, first+1, last)

print(summa([2,4,94])) # 100
