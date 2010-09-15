# exercise 1
# test answers, don't comment on them
ex1=[0, 1, 2, 1, 1, 0, 0]

# exercise 2

# first CSP exercise, see attached image for calculation order
ex2a=[
	{0:'r'},
	{0:'r', 1:'g'},
	{0:'r', 1:'g', 2:'r'},
	{0:'r', 1:'g', 2:'r', 3:'b'},
	{0:'r', 1:'g', 2:'r', 3:'b', 4:'g'},
	{0:'r', 1:'g', 2:'r', 3:'b', 4:'g', 5:'b'}
	]

# second CSP exercise, see attached image for calculation order
ex2b=[
	{0:'r'},
	{0:'r', 1:'g'},
	{0:'r', 1:'g', 3:'b'},
	{0:'r', 1:'g', 3:'b', 5:'g'},
	{0:'r', 1:'g', 3:'b', 5:'g', 2:'r'},
	{0:'r', 1:'g', 3:'b', 5:'g', 2:'r', 4:'g'}
	]

#exercise 3
# increasing BCD-"score" gave no pruned nodes. B was smallest => a1 is preferred move.

# max move
ex3a = ['a1']
# pruned nodes (none)
ex3b = []

