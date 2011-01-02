import random

class card:
	suit = None
	valor = None
	flipped = None
	def __init__(self, suit, valor):
		self.suit = suit
		self.valor = valor
		self.flipped = False

	def flip(self):
		self.flipped = not self.flipped
	def __str__(self):
		if self.flipped:
			return '(BACK)'
		out = '(' + self.suit + ': '
		if self.valor == 1:
			out += 'A'
		elif self.valor == 11:
			out += 'J'
		elif self.valor == 12:
			out += 'Q'
		elif self.valor == 13:
			out += 'K'
		else:
			out += str(self.valor)

		out += ')'
		return out

class deck:
	cards = None
	def __init__(self):
		self.cards = []
		for suit in ['D', 'H', 'C', 'S']:
			for valor in range(1, 13+1):
				self.cards.append(card(suit, valor))

	def __str__(self):
		out = '[ '
		for card in self.cards:
			out += str(card) + ' '
		out += ']'
		return out

	def shuffle(self):
		random.shuffle(self.cards)

	def size(self):
		return len(self.cards)

	def put(self, card):
		self.cards.append(card)

	def pick(self):
		return self.cards.pop() # tar bort och returnera sista elementet (kortet)

d = deck()

# ta ut det översta kortet, vänd på det och stoppa tillbaka det.
c = d.pick()
c.flip()
d.put(c)
print(d)
d.shuffle()
print(d)

