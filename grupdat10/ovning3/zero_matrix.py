def zero_matrix(n):
	mat = []
	for i in range(n):
		row = []
		for j in range(n):
			row.append(0)
		mat.append(row)

	return mat

print(zero_matrix(5))

def err_matrix(n):
	mat = []

	row = []
	for i in range(n):
		row.append(0)
	
	for j in range(n):
		mat.append(row)
	
	return mat

matrix = err_matrix(5)
print (matrix)
matrix[0][0] = 1
print (matrix)
