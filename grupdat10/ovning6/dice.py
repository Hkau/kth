import random

class coin:
	def __init__(self):
		self.flip()

	def flip(self):
		# ett eller noll, 50% chans
		if random.randint(0, 1) == 0:
			self.heads_up = True
		else:
			self.heads_up = False

	def heads(self):
		return self.heads_up

	def tails(self):
		return not self.heads()

	def __str__(self):
		if self.heads():
			return '(+)'
		else:
			return '(-)'

class dice:
	def __init__(self, size):
		self.size = size
		self.throw() # börjar med ett slumpat värde
	def throw(self): # or throw
		self.top = random.randint(1, self.size) # 1 t.o.m. size i storlek (d6 har 1..6
		# return self.top

	def read(self):
		return self.top

	def __str__(self):
		return str(self.read())

d = dice(20)

for i in range(100):
	d.throw()
	print(d.read())

for i in range(40):
	print(coin())

