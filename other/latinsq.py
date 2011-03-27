# latinska kvadrater av ordning 4

import sys

def printsq(sq, sq2):
	print()
	for i in range(len(sq)):
		print (sq[i], sq2[i])

def solve_ortagonal(sq):
	fakesq = []

	for i in range(len(sq)):
		arr = []
		for j in range(len(sq)):
			arr.append(-1)
		fakesq.append(arr)
	return solve_internal(sq, fakesq)

def solve_internal(sq, sq_solve, x = 0, y = 0):
	for num in range(len(sq)):
		sq_solve[x][y] = num

		sol_sq = sq[x][y]
		if sol_sq < num:
			pair = (sol_sq, num)
		else:
			pair = (num, sol_sq)

		invalid = False
		for i in range(x):
			if sq_solve[i][y] == num:
				invalid = True
				break

		if invalid:
			continue

		for i in range(y):
			if sq_solve[x][i] == num:
				invalid = True
				break

		if invalid:
			continue

		for i in range(x):
			for j in range(y+1):
				if sq[i][j] < sq_solve[i][j]:
					pair2 = (sq[i][j], sq_solve[i][j])
				else:
					pair2 = (sq_solve[i][j], sq[i][j])

				if pair == pair2:
					# finns redan, shortcircuit
					invalid = True
					break
			if invalid:
				break
		if invalid:
			continue


		if x == y and y == len(sq) - 1:
			printsq(sq, sq_solve)
		else:
			next_x = x + 1
			if next_x == len(sq):
				next_y = y + 1
				next_x = 0
			else:
				next_y = y

			solve_internal(sq, sq_solve, next_x, next_y)
	sq_solve[x][y] = -1

def run_squares(sq, x = 0, y = 0):
	for num in range(len(sq)):
		sq[x][y] = num
		invalid = False

		for i in range(x):
			if sq[i][y] == num:
				invalid = True
				break

		if invalid:
			continue

		for i in range(y):
			if sq[x][i] == num:
				invalid = True
				break

		if invalid:
			continue

		if x == y and y == len(sq) - 1:
			solve_ortagonal(sq)
		else:
			next_x = x + 1
			if next_x == len(sq):
				next_y = y + 1
				next_x = 0
			else:
				next_y = y

			run_squares(sq, next_x, next_y)

try:
	n = int(sys.argv[1])

	fakesq = []

	for i in range(n):
		arr = []
		for j in range(n):
			arr.append(-1)
		fakesq.append(arr)

	run_squares(fakesq)

except (IndexError, ValueError):
	print("usage:", sys.argv[0], "n")

