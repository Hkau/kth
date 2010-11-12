class stack:
	lista = None
	def __init__(self):
		self.lista = [] # stacken tom från början

	def isempty(self):
		return len(self.lista) is 0

	def push(self, item):
		self.lista.append(item)

	def pop(self):
		return self.lista.pop()

s = stack()

s.push(1)
s.push(2)
s.push(3)

while not s.isempty():
	print(s.pop())

# baklänges utskrivet! :)

print('--')

from queue import Queue # i modulen queue finns en klass som heter Queue, så det så.

q = Queue()

q.put(1)
q.put(2)
q.put(3)

while not q.empty():
	print(q.get())


# uppgift 82. vill man vända på en kö lägger man hela kön på en stack och lägger sedan tillbaka varje element i kön

