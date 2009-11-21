#rövarspråket
konsonanter = 'bcdfghjklmnpqrstvwxzBCDFGHJKLMNPQRSTVWXZ'
vokaler = 'aeiouyAEIOUY'


def rovarsprak(str):
	utstr = ''
	for c in str:
		utstr+=c
		if c in konsonanter:
			utstr+='o'
			utstr+=c.lower()
	return utstr

print(ord("f"))

"I'm so cool that I write diz shit yo'!"
'And thus he quoted "This is a double-fnutted quote, yo!"'
print ('''
This is a tripplefnutt, anything goes! D:

"""
newlines and tabs are textz too! \n asdf
				
''')

def fikonsprak(str):
	utstr = ''
	ordlista = str.split()

	for ord in ordlista:
		for i in range(len(ord)):
			if ord[i] in vokaler:
				utstr += 'fi' + ord[i+1:]
				utstr += ord[0:i+1] + 'kon'

				break
		utstr += ' '
	return utstr

# MENYN:

class foo:
	def __init__(self):
		print('Hello, Emil!')

f = foo()

print('Meny: 1 = rövarspråket')

val = int(input('Välj språk'))

text = input('Rad att översätta: ')

if val == 1:
	print('Rövarspråk: ', rovarsprak(text))
elif val == 2:
	print('Fikonspråk: ', fikonsprak(text))
# ...
else:
	print('Ogiltigt val! :)')
