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

# kopiera alla icke-tomma rader inte är en kommentar (inte börjar på % eller =)

for line in indata:
	line = line.strip()
	if len(line) == 0:
		continue
	if line[0] == '%' or line[0] == '=':
		continue
	varudata.append(line)

print(varudata) # för att se att allt gått rätt, åäö ser konstigt ut, är ok.

varor = [] # fyll på med varor här

for i in range(0, len(varudata), 3): # på var tredje rad börjar en ny vara
	code = int(varudata[i])
	name = varudata[i+1]

	info = varudata[i+2].split()
	price = float(info[0])
	stock = int(info[1])

	v = vara(code, name, price, stock)
	print(v) # se till att vi har läst in allting rätt!
	varor.append(v)

# varor innehåller nu alla varor som finns i lager.

print('===')
# Del 2, använda systemet + kvitto
print('Varunamn', 'Antal', 'A-pris', 'Summa')
print('===')

while True:
	# läs in från kassören
	line = input()

	if line == '#':
		break

	tokens = line.split()

	kod = int( tokens[0] ) # behöver kollas för betyg C
	if len(tokens) == 2: # behöver kolla att len inte > 2 för betyg C
		antal = int( tokens[1] ) # behöver kollas för betyg C, negativa kan vara ok för "återköp" (vilket är del av betyg B)
	else:
		antal = 1

	# hitta varan

	artikel = None
	for v in varor:
		if v.code == kod:
			artikel = v
			break

	if artikel == None: # förmodligen del av betyg C
		print('varan finns inte i systemet')
		continue

	if artikel.stock < antal: # detta är del av betyg C, men behöver inte introducera try/except
		print('Det finns inte', antal, 'av',  artikel.name, 'i lager!')
		continue

	# 'köp varan'
	artikel.stock -= antal
	print(artikel.name, antal, artikel.price, artikel.price * antal)

# DEL 3: Spara lagerstatus till fil!

varufil = open('varor.txt', 'w')

for v in varor:
	varufil.write(str(v.code) + '\n')
	varufil.write(v.name + '\n')
	varufil.write(str(v.price) + ' ' + str(v.stock) + '\n')

varufil.close()

