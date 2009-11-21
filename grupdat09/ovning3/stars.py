def writestars(n):
	for i in range(n):
		print('*', end='')
	print()

writestars(17)

def write_vertstars(n):
	for i in range(n):
		print('*', end='\v')
	print(end='\r')

write_vertstars(17)

writestars(17)

for i in range(20):
	for j in range(i):
		print(j, end='')
	print(end='\r')
print()

n = 42
fak = 1
for i in range(1, n+1):
	fak *= i

print('Fak:', fak)

