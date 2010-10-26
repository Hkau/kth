# 61. Skriv en procedur skrivstora(v) som skriver
#     ut alla tal i v som är större än en miljon.

def skrivstora(v):
	for tal in v:
		if tal > 1000000:
			print(tal)

skrivstora([1,32,43,2012301240,2034, 1000**2+1])

