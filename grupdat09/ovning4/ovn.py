foo = [1,3,2,5,5]

for elem in foo:
	elem += 4

for i in range(len(foo)):
	foo[i] += 4

print(foo)

foo = "hej"

foo2 = 3*[foo]

foo2[0] += "san"

print(foo2)


lists = [[2]]*3
lists[0][0]+= 2
print(lists)

lists = [[1,2,3,4],[5,6,7,8],[1,2,3,4],[5,6,7,8]]

for list in lists:
	for elem in list:
		print(elem)

del list
text = "abcd"

for i in range(len(text)):
	print(text[i:] + text[:i])

listtest = ["asdf", 1, 42, True]

print(listtest)

string = "borde inte plugga"
string = string[:5] + string[10:]

print(string)

print(str(list(string)))

lista = list(range(1, 24+1))

while len(lista) > 0:
	print(str(lista.pop())+ ": " + str(sum(lista)))


# ---- FIBONACCI-TALSERIEN ---- #

fib = [0, 1]

for i in range(100):
	fib.append(fib[-1]+fib[-2])

print(fib)

# ---- PRIMTAL ---- #

prim = [2]

for i in range(3, 10000):
	for j in range(len(prim)):
		if prim[j]**2 > i:
			prim.append(i)
			break
		if (i % prim[j]) == 0:
			break

print(prim)
listan = [2,3]
listan.insert(0, 1)
print(listan)

# ---- RÖVARSPRÅKET⁻¹ ---- #

sprak = "rorövovarorsospoproråkoketot"
konsonanter = "bcdfghjklmnpqrstvwxz"

def from_rovarspraket(text):
	utrad = ""

	i = 0
	while i < len(text):
		utrad += text[i]
		if text[i].lower() in konsonanter:
			i+= 3
		else:
			i+= 1
	return utrad

print(from_rovarspraket(sprak))	
