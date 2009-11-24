from queue import Queue
from bintree import Bintree

ordl = open('word3').read().split()

svenska = Bintree()
gamla = Bintree()

for o in ordl:
	svenska.put(o)

alfabet = "qweryuiopåasdfgjklöäzxcvbnm"

bfsqueue = Queue()

class bfsnode:
	value=None
	parent=None

	def __init__(self, value, parent):
		self.value = value
		self.parent = parent

def makesons(o, end):
	word = o.value
	for c in alfabet:
		for i in range(len(word)):
			if c == word[i]:
				continue
			child = word[:i] + c + word[i+1:]

			if child == end:
				return bfsnode(child, o)

			if not gamla.exists(child) and svenska.exists(child):
				gamla.put(child)
				bfsqueue.put(bfsnode(child, o))
	return None

def bfs(start, end):
	bfsqueue.put(bfsnode(start, None))

	while not bfsqueue.isempty():
		nod = makesons(bfsqueue.get(), end)

		if nod is not None:
			return nod

	return None

def writechain(son):
	if son is None:
		return
	writechain(son.parent)
	print(son.value)

fran = input('från? ')
till = input('till? ')

nod = bfs(fran, till)

if nod is not None:
	writechain(nod)
else:
	print('det finns ingen väg från', fran, 'till', till)

