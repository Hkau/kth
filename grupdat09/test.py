# coding=utf8
import random

foo = ["bar", "baz", "'foo'", "asdf", "fdsa"]
print foo
def main():
	global foo
	utfil = file("log.txt", "w")

	print >> utfil, "Hello, World! :)"
	print "☃, ÅÄÖåäö funkar eftersom Python är fett nice™! :D"

	print """
		This Cool Message contains unescaped newlines! :D
	"""

	print "last:", foo[-1]

	print "uppercase-lista index 1->sista:", map(str.upper, foo[1:len(foo)])

#	print "Hej, skriv in ditt namn:",
#	namn = raw_input()
#
#	asdf = raw_input("Skriv in nåt ballt!")
#
#	tal = input("Skriv om ditt tal[:)]: ")
#	if tal == 42:
#		print "bra val <3"
#	elif tal == 1337:
#		print "tönt :>"
#	else:
#		print "#"
#
#	print "Ryktet™ säger att du heter", namn, "och gissade", tal
	if True and True:
		print "yo"
	if False or True:
		print "stuff!"

	foolean = False
	while not foolean:
		print "foolean isn't true!?"
		foolean = random.randrange(0,3) == 0

	print "Listan innehöll:"

	for word in foo:
		print '\t' + word

	foo = ["asdf"]
	print "och innehåller nu:"
	print foo

	while True:
		try:
			x = input("Gissa mitt tal: ")

			if x == 4711:
				print "Rätt! Du gissar väldigt bra, ser jag!"
				break
			print "Fel.",
		except:
			print "Ogiltigt tal. :("

	assert True

	bar = foo

main()

