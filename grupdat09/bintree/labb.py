
from bintree import Bintree

f = open("word3")

ordlista = f.readlines()

for i in range(len(ordlista)):
	ordlista[i] = ordlista[i].rstrip()

swedish = Bintree()

for ord in ordlista:
	swedish.put(ord)

f.close()

f = open("english")

ordlista = f.read().split()

oldtree = Bintree()

for i in range(len(ordlista)):
	ordlista[i] = ordlista[i].strip('",.')

print (ordlista)

for o in ordlista:
	if oldtree.exists(o):
		continue
	if swedish.exists(o):
		print(o)
		oldtree.put(o)

