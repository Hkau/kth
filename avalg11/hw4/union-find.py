#!/usr/bin/env python
class Node:
	def __init__(self, id):
		self.id = str(id)
		self.parent = self
		self.rank = 0
	def __str__(self):
		return '(' + self.id + ',' + self.parent.id + ')'

def makeSet(i):
	return Node(i)

def union(x, y):
	x = find(x)
	y = find(y)
	if x == y:
		return

	if x.rank < y.rank:
		x.parent = y
	else:
		y.parent = x
		if x.rank == y.rank:
			x.rank += 1

def find(x):
	if x.parent == x:
		return x
	x.parent = find(x.parent)
	return x.parent

foo = []

# for (i = 1; i <= 16; i++)
#     makeSet(i)
for i in range(16):
	foo.append(makeSet(i+1))
for p in foo:
	print p,
print

# for (i = 1; i <= 15; i += 2)
#     union(i, i+1)
for i in range(0, 15, 2):
	union(foo[i], foo[i+1])
for p in foo:
	print p,
print

# for (i = 5; i <= 13; i += 4)
#     union(i, i+2)
for i in range(4, 13, 4):
	union(foo[i], foo[i+2])
for p in foo:
	print p,
print

# union(3, 15)
union(foo[2], foo[14])

#union(10,8)
union(foo[9], foo[7])

#union(16,2)
union(foo[15], foo[1])

#union(13,9)
union(foo[12], foo[8])

for p in foo:
	print p,
print

