from bintree import Bintree

tree=Bintree()
print("Putta in ord, avsluta med retur")
while True:
	word=input()
	if word=="": break
	tree.put(word)
	print("Hela trädet:")
	tree.write()
	print("Sök ord, avsluta med retur")
	while True:
		word=input()
		if word=="":
			break
		if tree.exists(word):
			print(word,"finns")

