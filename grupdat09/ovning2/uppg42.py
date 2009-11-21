import random
slump = random.randint(1, 100)

gissa = 0
while gissa != slump:
	gissa = int(input('Gissa talet: '))
	if gissa > slump:
		print('För stort!')
	elif gissa < slump:
		print('För litet!')

print('Bra gissat! :)')

