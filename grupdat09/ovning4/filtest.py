f = open("fil", "r")
rader = f.readlines()

for i in range(len(rader)):
	rader[i] = rader[i].strip()

print(rader)
