# 62. Skriv en procedur skrivkorta(txt) som skriver ut alla
#     ord i textsträngen txt som har färre än 5 bokstäver.

def skrivkorta(txt):
	ordlista = txt.split()
	for ord in ordlista:
		if len(ord) < 5:
			print(ord)

skrivkorta("det var en gång en, liten fågel, ja en fågel, han bodde på landet, och roger hette han.")

