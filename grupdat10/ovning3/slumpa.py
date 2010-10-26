# listövning, plocka ut slumpmässigt element ur en lista.
import random

def slumpa(v):
	random_index = random.randint(0, len(v)-1)
	return v[random_index]

print(slumpa([42, 4, 94]))
