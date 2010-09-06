# [time, sol_depth, max_depth]
A = [2,8,11]
B = [5,3,5]

sp = 46
ms = 6
#You are designing a search algorithm for solving a maze. You should analyse the performance of two different movement sets. The movement set A has 2 different kinds of movements. With that set the maze can be solved in 8 movements ( i.e. there is a solution at level 8) and the search tree has a maximum depth of 11( i.e. there are no nodes on level 12). The same data for movement set B is [5, 3, 5]. Your computer's memory  can hold 46 nodes, and generating a node takes 6 microseconds. Following the standards in the book about which nodes are kept in memory,  calculate the time spent in solving the maze and the theoretical maximum memory consumption in terms of nodes in the worst case scenario for breadth-first, standard depth-first and iterative deepening depth-first. If that consumption fits into memory, (i.e. if the worst scenario can be solved with that amount of memory) show also the contents of the memory when the solution is found (see attachment answers.txt to check the format of the answer). The nodes are identified by a number starting at 1 (root), and within a level, lower numbered nodes are expanded before higher numbered ones.

# [time,max memory consumption, [sequence of nodes in memory when solution is found, empty if didn't fit]]

def count_nodes(b, d): # fult :)
	sum = 0
	for i in range(d):
		sum += b**i
	return sum

def gen_bfs(x):
	print '[',
	print count_nodes(x[0], x[1])*ms, ',',
	print count_nodes(x[0], x[1]),
	print ',',
	if count_nodes(x[0], x[1]) > sp:
		print '[ ]', # bfs too large
	else:
		print '[',
		for i in range(1, count_nodes(x[0], x[1])):
			print i, ',',
		print count_nodes(x[0], x[1]),
	print ']',

def dfs_path(b, d):
	print '[',
	node = 0
	for i in range(d-1):
		node += b**i
		print node, ',',
	print node + b**(d-1),
	print ']',

def gen_dfs(x):
	print '[',
	exp_nodes = count_nodes(x[0], x[2]) - (count_nodes(x[0], x[2] - x[1]) - 1) # tree except for tree beneath solution at sol_depth
	print exp_nodes * ms, ',',
	print x[2], ',',
	dfs_path(x[0], x[1])
	print ']',

def gen_ids(x):
	print '[',
	time_nodes = 0
	for i in range(1,x[1]+1): # iterative deepening
		time_nodes += count_nodes(x[0], i)
	print time_nodes * ms, ',',
	print x[1], ',',
	dfs_path(x[0], x[1])
	print ']',

def gen_res(x):
	# switch to one-indexing, root node is at depth 0 not 1, according to task info (depth = number of moves)
	x[1]+=1
	x[2]+=1
	print '[',
	gen_bfs(x)
	print ',',
	gen_dfs(x)
	print ',',
	gen_ids(x)
	print ']',

gen_res(A)
print
gen_res(B)

