# 65. Skriv en funktion median(u) som returnerar medianen
#     (mellersta värdet) av en godtycklig talvektor.

def median(u):
	v = u
	v.sort()

	if len(v) % 2 == 1: # om den är udda
		mitten = len(v) // 2
		return v[mitten]

	tal1 = (len(v)-1) // 2
	tal2 = len(v) // 2

	return (v[tal1] + v[tal2]) / 2

print(median([2,7,42])) # 7
print(median([0,4,7,1235])) # 5.5

