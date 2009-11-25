# HashTrie, a mashup between hash tables and tries.
# Made by Peter Boström, 2009. No rights reserved. 
#
# Made because I couldn't find something with this
# approach, but it's simple enough that someone
# should've done it already. And for fun. :)
#

class LinkedNode:
	hash_code = None
	value = None
	next = None

	def __init__(self, hash_code, value):
		self.hash_code = hash_code
		self.value = value
		self.next = None

class HashNode:
	hash_code = None
	table = None

	def __init__(self, parent):
		self.hash_code = -1
		self.table = [None]*0x100

def unsigned_hash(obj):
	hash_code = hash(obj)
	hash_code <<= 1

	if hash_code < 0:
		hash_code = -hash_code + 1

	return hash_code

class HashTrie:
	hash_node = None
	entries = None

	def __init__(self):
		self.hash_node = HashNode(None)
		self.entries = 0

	def put(self, obj):
		hash_code = unsigned_hash(obj)

		node = self.hash_node

		shift_count = 0

		while True:
			mask = (hash_code >> shift_count) & 0xFF

			if node.table[mask] is None:
				node.table[mask] = LinkedNode(hash_code, obj)
				return True

			if node.table[mask].hash_code == -1:
				node = node.table[mask]
				shift_count +=8
				continue

			if hash_code != node.table[mask].hash_code:
				linked_node = node.table[mask]
				node.table[mask] = HashNode(node)
				node = node.table[mask]
				shift_count += 8
				node.table[(linked_node.hash_code >> shift_count) & 0xFF] = linked_node
				continue

			node = node.table[mask]

			while node != None:
				if node.value == obj:
					return False

			node.next = LinkedNode(hash_code, obj)

			return True

	def exists(self, obj):
		hash_code = unsigned_hash(obj)

		node = self.hash_node
		hash_left = hash_code

		while node != None:
			mask = hash_left & 0xFF
			if node.hash_code == -1:
				node = node.table[mask]
				hash_left >>= 8
				continue

			if node.hash_code != hash_code:
				return False

			while node != None:
				if node.value == obj:
					return True
				node = node.next
		return False

	def remove(self):
		pass
		# do diz shit yo :)

	def empty(self):
		self.hash_node = HashNode()

