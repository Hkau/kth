def bsearch(v, x):
	if len(v) == 0:
		return None
	return bin_rec(v, x, 0, len(v)-1) # anropa hjälpmetod

def bin_rec(v, x, first, last): # finns x inom intervallet first.. inkl. last inuti den sorterade listan v?
	print("bin_rec", first, last)
	if first == last: # basfall!
		if v[first] == x:
			return first
		return None
	mid = (first + last) // 2 # avrundat neråt
	print("mid:", mid)

	if v[mid] < x:
		print("mid < x")
		return bin_rec(v, x, mid+1, last)
	else:
		print("mid >= x")
		return bin_rec(v, x, first, mid)

print(bsearch([0,3,5,7], 5))
