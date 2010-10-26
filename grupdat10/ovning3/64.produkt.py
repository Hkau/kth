# 64. Skriv en funktion produkt(u,v) som returnerar
#     skalärprodukten av två trekomponentersvektorer.

def produkt(u,v):
	prod = u[0]*v[0] + u[1]*v[1] + u[2]*v[2]
	return prod

