# Spåren A, B och C representeras av tre abstrakta stackar . De n vagnarna står
# från början på spår A och ordningen ges av listan spooktrain (t ex 3,1,4,2).
# Ange utförligt en algoritm som ordnar vagnarna (i exemplet till 4,3,2,1).

spooktrain = [3,1,4,2]

class stack:
	def __init__(self, items):
		self.list = items
	def pop(self):
		return self.list.pop()
	def push(self, item):
		self.list.append(item)
	def isempty(self):
		return len(self.list) == 0

A = stack(spooktrain) # [3,1,4,2]
B = stack([])
C = stack([])

#print(A.list, B.list, C.list)

# först ska vi hista första vagnen 1
find = 1
while not A.isempty():
	print('A:', A.list)
	print('B:', B.list)
	print('C:', C.list)
	print()

	# kör fram en vagn från spår A
	item = A.pop()

	if item == find:
		# vagnen är den vi letar efter, lägg den i spår C
		C.push(item)

		# ställ tillbaka den del av tåget som inte är sorterad på spår A från spår B
		while not B.isempty():
			print('A:', A.list)
			print('B:', B.list)
			print('C:', C.list)
			print()
			A.push( B.pop() )

		find += 1 # säg till att vi letar efter nästa vagn
	else:
		# vagnen är nån vi inte letar efter, lägg den på spår B
		B.push(item)

# ställ tillbaka tåget på spår A
while not C.isempty():
	A.push(C.pop())

print('A:', A.list)
print('B:', B.list)
print('C:', C.list)
print()

#A: [3, 1, 4, 2]
#B: []
#C: []

#A: [3, 1, 4]
#B: [2]
#C: []

#A: [3, 1]
#B: [2, 4]
#C: []

#A: [3]
#B: [2, 4]
#C: [1]

#A: [3, 4]
#B: [2]
#C: [1]

#A: [3, 4, 2]
#B: []
#C: [1]

#A: [3, 4]
#B: []
#C: [1, 2]

#A: [3]
#B: [4]
#C: [1, 2]

#A: []
#B: [4]
#C: [1, 2, 3]

#A: [4]
#B: []
#C: [1, 2, 3]

#A: [4, 3, 2, 1]
#B: []
#C: []


