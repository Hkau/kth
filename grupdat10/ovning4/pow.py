def pow(x, n):
	if n == 0: # basfall
		return 1
	return x * pow(x, n-1) # n minskar, mindre delproblem

print(pow(4, 4)) # 256
