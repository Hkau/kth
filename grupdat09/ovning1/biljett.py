# coding=utf8
barn = int(raw_input("Antal barn: "))
vuxna = int(raw_input("Antal vuxna: "))

betalt = float(raw_input("Betalt: "))

kostnad = barn*13.50 + vuxna*20.00

if betalt < kostnad:
	print "Det räcker inte! :("
elif betalt == kostnad:
	print barn+vuxna, "biljetter."
else:
	print barn+vuxna, "biljetter och", '%.2f' % (betalt-kostnad), "tillbaka."

