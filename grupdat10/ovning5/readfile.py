import codecs
fil = codecs.open('ordlista2', 'r', 'iso8859')
# eller
fil = open('ordlistautf')

# fil:
# afton
# akter
# andel

ordlista = fil.readlines()
fil.close()

for ord in ordlista:
	print (ord)

# ger:
# ord
#
# ord2
# 
# osv
#

for i in range( len(ordlista) ):
	ordlista[i] = ordlista[i].strip()

for ord in ordlista:
	print(ord)

# ger:
# ord
# ord2
# osv

