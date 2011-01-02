# coding=utf8
# Varuprisdatabas, P-uppgift nummer 145

class vara:
	def __init__(self, code, name, price, stock):
		self.code = code
		self.name = name
		self.price = price
		self.stock = stock

	def __str__(self):
		return self.name + "! code: " + str(self.code) + ", price: " + str(self.price) + ", stock: " + str(self.stock)

# DEL 1: Inmatning
varufil = open('varor.txt')
indata = varufil.readlines()
varufil.close()

varudata = []

# kopiera alla icke-tomma rader inte �r en kommentar (inte b�rjar p� % eller =)

for line in indata:
	line = line.strip()
	if len(line) == 0:
		continue
	if line[0] == '%' or line[0] == '=':
		continue
	varudata.append(line)

print(varudata) # f�r att se att allt g�tt r�tt, ��� ser konstigt ut, �r ok.

varor = [] # fyll p� med varor h�r

for i in range(0, len(varudata), 3): # p� var tredje rad b�rjar en ny vara
	code = int(varudata[i])
	name = varudata[i+1]

	info = varudata[i+2].split()
	price = float(info[0])
	stock = int(info[1])

	v = vara(code, name, price, stock)
	print(v) # se till att vi har l�st in allting r�tt!
	varor.append(v)

# varor inneh�ller nu alla varor som finns i lager.

print('===')
# Del 2, anv�nda systemet + kvitto
print('Varunamn', 'Antal', 'A-pris', 'Summa')
print('===')

while True:
	# l�s in fr�n kass�ren
	line = input()

	if line == '#':
		break

	tokens = line.split()

	kod = int( tokens[0] ) # beh�ver kollas f�r betyg C
	if len(tokens) == 2: # beh�ver kolla att len inte > 2 f�r betyg C
		antal = int( tokens[1] ) # beh�ver kollas f�r betyg C, negativa kan vara ok f�r "�terk�p" (vilket �r del av betyg B)
	else:
		antal = 1

	# hitta varan

	artikel = None
	for v in varor:
		if v.code == kod:
			artikel = v
			break

	if artikel == None: # f�rmodligen del av betyg C
		print('varan finns inte i systemet')
		continue

	if artikel.stock < antal: # detta �r del av betyg C, men beh�ver inte introducera try/except
		print('Det finns inte', antal, 'av',  artikel.name, 'i lager!')
		continue

	# 'k�p varan'
	artikel.stock -= antal
	print(artikel.name, antal, artikel.price, artikel.price * antal)

# DEL 3: Spara lagerstatus till fil!

varufil = open('varor.txt', 'w')

for v in varor:
	varufil.write(str(v.code) + '\n')
	varufil.write(v.name + '\n')
	varufil.write(str(v.price) + ' ' + str(v.stock) + '\n')

varufil.close()

