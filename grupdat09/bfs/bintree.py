class Bintree:
	top = None

	def put(self, x):
		if self.top == None:
			self.top = Node(x)
			return True
		return self.top.put(x)

	def exists(self, x):
		if self.top == None:
			return False
		return self.top.exists(x)

	def write(self):
		if self.top != None:
			self.top.write()

class Node:
	left = None
	right = None
	value = None

	def __init__(self, x):
		self.value = x

	def put (self, x):
		if x == self.value:
			return False

		if x < self.value:
			if self.left != None:
				return self.left.put(x)
			self.left = Node(x)
			return True

		if self.right != None:
			return self.right.put(x)
		self.right = Node(x)
		return True

	def exists (self, x):
		if x == self.value:
			return True

		if x < self.value:
			if self.left == None:
				return False
			return self.left.exists(x)

		if self.right == None:
			return False
		return self.right.exists(x)

	def write(self):
		if self.left != None:
			self.left.write()

		print (self.value)

		if self.right != None:
			self.right.write()

