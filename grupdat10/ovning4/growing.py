def growing(v):
	prev = v[0]
	for i in range(1, len(v)):
		if v[i] < prev: # senare element mindre, listan alltså ej växande
			return False
		prev = v[i]
	return True

# annan variant

def growing_recursive(v, first, last, prev):
	if first == last: # bara ett element kvar att kolla, basfall
		if v[first] < prev:
			return False
		return True
	if v[first] < prev:
		return False
	return growing_recursive(v, first+1, last, v[first])
def growing(v):
	return growing_recursive(v, 1, len(v)-1, v[0])

print("Growing: ", growing([2,2,4]))

