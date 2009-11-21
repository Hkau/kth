# coding=utf8
pinne = [42,42,42]
pinne[0] = float(input('Pinne 1: '))
pinne[1] = float(input('Pinne 2: '))
pinne[2] = float(input('Pinne 3: '))

pinne.sort()

if pinne[0] + pinne[1] < pinne[2]:
	print("Går inte! :(")
else:
	print("Funkar finfint! :)")

