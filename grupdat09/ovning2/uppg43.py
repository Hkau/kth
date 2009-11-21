summa_betyg = 0
antal_betyg = 0

while True:
	betyg = int(input('Betyg: '))
	if betyg == 0:
		break

	antal_betyg += 1
	summa_betyg += betyg

if antal_betyg == 0:
	print('Inga betyg inmatade.')
else:
	print('Medel betyg är', summa_betyg / antal_betyg)
