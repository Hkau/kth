# coding=utf8
x = 5
print x

def change_x(val):
	global x
	x = val
	return x

def add_x():
	global x
	y = input("Addera X med absolutbeloppet av vad? ")
	x += abs(y)
	print "X är nu", x

add_x()
