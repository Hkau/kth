#Av: Cecilia Bruhn, fanextra
fil=open("word3")
ordlista=fil.read().split()

from bintree import Bintree
from queue import Queue

q2=Queue()
q=Queue()
svenska=Bintree()           #träd med ordlistan

for ord in ordlista:
    svenska.put(ord)        #stoppar in orden från ordlistan i svenska binärträdet och skriver ut dubletterna

gamla=Bintree()             #träd med alla besökta ord

class Node:					#skapar en ny nod klass
    value=None
    parent=None
    def __init__(self, value, parent):		#ser till att noden tar fler än en parameter
        self.value=value
        self.parent=parent

#startord=Node(None,None)
#slutord=Node(None,None)
startord=input("Ordet där du vill börja:")
#slutord=input("Ordet du vill gå till:")
      
def makesons(nod):
    alfabetet="qwertyuiopåasdfghjklöäzxcvbnm"
    for tecken in alfabetet:
        for i in range(len(nod.value)):
            child=nod.value[:i]+tecken+nod.value[i+1:]
            if svenska.exists(child) and not gamla.exists(child):
                nod2=Node(child, nod)			#skapar vi en ny nod med child som ord(value) och pekar(parent) på nod
                gamla.put(child)				#stoppar in de besökta child i binärträdet gamla
                q.put(nod2)						#stoppar in nod2 i kön

def countchain(nod2):
	if nod2.parent==None:
		return 0 
	else: 
		return 1+countchain(nod2.parent)

def writechain(nod2):
	if nod2.parent==None:
		print(nod2.value)
		return
	writechain(nod2.parent)
	print(nod2.value)

q.put(Node(startord,None))			#stoppar in startord i kön 
while not q.isempty():	#så länge kön inte är tom...
	word=q.get()		#Lagrar q.get() som en variabel
	makesons(word) 		#Kör makesons på alla ord man stoppar in i kön, dvs sönerna till starordet och sönernas söner osv. Tills man ej hittar några söner längre. Det sista ordet som man då tog ut i kön är det ordet som ger den längsta vägen till gud.
writechain(word)		#När kön är tom så skriver man ut längsta kedjan till/från gud.
print("Kedjans längd:", countchain(word))	#Skriver ut längsta kedjans längd.




	
	
