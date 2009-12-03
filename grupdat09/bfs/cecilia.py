fil=open("word3.txt")
ordlista=fil.read().split()
from bintree import Bintree
svenska=Bintree()           #träd med ordlistan
for ord in ordlista:
	svenska.put(ord)        #stoppar in orden från ordlistan i svenska binärträdet
gamla=Bintree()             #träd med alla besökta ord
startord=input("Ordet där du vill börja:")
slutord=input("Ordet du vill gå till:")
def makesons(o):
	alfabetet="qwertyuiopåasdfghjklöäzxcvbnm"
	for tecken in alfabetet:
		for i in range(len(o)):
			child=o[:i]+tecken+o[i+1:]
			if svenska.exists(child)and not gamla.exists(child):
				gamla.put(child)
				print(child)

makesons(startord)
