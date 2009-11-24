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

