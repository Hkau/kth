# coding=utf8

'''
	Ta upp # coding=utf8/latin1
	15/2, 15/2.0, 15.0/2, 15.0/2.0,
	int: 15 // 2, float: 15//2.0
	
'''

print "---PYTHONS NYBURGARE---"
def nyburgare():
	burgare = int(raw_input('Önskat antal burgare: '))
	pommes = int(raw_input('Önskat antal pommesfrites: '))
	cola = int(raw_input('Önskat antal cola: '))

	print 'Det blir', 30 * burgare + 10 * pommes + 5 * cola, 'kronor, tack!'

while True:
	try:
		nyburgare()
		break
	except ValueError:
		print 'Det där var inget bra antal! :<'
		print '\nBörja om!\n'
