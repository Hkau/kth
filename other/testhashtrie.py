from hashtrie import HashTrie
from datetime import datetime
ordlista = open('words.txt').read().split()

print('lägger in')

print(datetime.now())
for i in range(100):
	ht = HashTrie()

	for o in ordlista:
		if not ht.put(o):
			print(o)
print(datetime.now())

print('lägger in dictionary')


for i in range(100):
	dictionary = dict()

	for o in ordlista:
		dictionary[o] = 0

print(datetime.now())
