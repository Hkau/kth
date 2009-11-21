# snacka om rekursivitet

# rekursiv minsta värdet i en lista eller största

# rekursiv fibonacci ( f(n) = f(n-1) + f(n-2)
# f(1) = 0, f(2) = 1

def fib(n):
	if n <= 1:
		return 0
	if n == 2:
		return 1
	return fib(n-1) + fib(n-2)

for i in range(24):
	print("fib", i, "=", fib(i))

#7.1 ge en rekursiv tanke för binärsökning!

#7.2 ge en rekursiv tanke för antalet siffror i heltalet n

print("rekursionsteckenräkning")

i = 2034
print (i)

def numcount(i):
	if i < 10:
		return 1
	return numcount(i//10) + 1

print(numcount(i))

# 7.3 fakultet

def fakultet(n):
	if n <= 1:
		return 1
	return n*fakultet(n-1)

print("fakultet för 42:", fakultet(42))

# men stacka 'rå! :)

# klasser!

# kom ihåg foo = VALUE (None, 0, 123456)

class stack:

	elem = []

	def __init__(self):
		print("Vi har en ny stack :)") # Dåligt/dumt att skriva. :)

	def push(self, x):
		self.elem.append(x)

	def pop(self):
		return self.elem.pop()

	def isempty(self):
		return len(self.elem) == 0

stacken = stack()

for num in range(10):
	stacken.push(num)

while not stacken.isempty():
	print(stacken.pop())

class Node:
	value = None
	next = None

	def __init__(self, x):
		self.value = x

class Queue:

	first = None
	last = None

	def put(self, x):
		node = Node(x)
		if self.first == None:
			self.first = self.last = node
		else:
			self.last.next = node
			self.last = node

	def get(self):
		tmp = self.first.value
		if self.first == self.last:
			self.first = self.last = None
		else:
			self.first = self.first.next
		return tmp

	def isempty(self):
		return self.first == None

print('Kö:')
que = Queue()

for num in range(10):
	que.put(num)

while not que.isempty():
	print(que.get())

