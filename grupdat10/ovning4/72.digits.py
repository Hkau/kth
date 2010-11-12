def digits(n):
	if n == 0: #basfall, viktigt, annars avslutas aldrig rekursionen
		return 0
	return 1 + digits(n//10) # anropar digits på ett strikt _mindre_ delproblem

print(digits(72019)) # 5
