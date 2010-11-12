def maximum(v):
	large = v[0]
	for i in range(1, len(v)):
		if v[i] > large:
			large = v[i]
	return large

print(maximum([20, 30, 10, 42, 0]))

# alt.

def maximum(v):
	if len(v) == 1:
		return v[0]
	return maximum_recursive(v, 1, len(v) - 1, v[0])

def maximum_recursive(v, first, last, prev_max):
	if first == last:
		if v[first] > prev_max:
			return v[first]
		else:
			return prev_max

	if v[first] > prev_max:
		return maximum_recursive(v, first+1, last, v[first])
	else:
		return maximum_recursive(v, first+1, last, prev_max)

print(maximum([24,34,0,23]))
