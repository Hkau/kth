import sys

for c in sys.stdin.read():
	if c != ' ':
		sys.stdout.write(c)
