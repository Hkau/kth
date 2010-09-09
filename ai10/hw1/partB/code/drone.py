import Queue
import copy
import sys

n = 11
target = 5
start_x = 0 # starts in left corner
start_y = n-1 # starts at bottom

field = [['#']*n] # box
#field = [[u'\u2744']*n] # flake

# deepcopy copies rows instead of appending references
for i in range(n):
	field.append(copy.deepcopy(field[0]))

# start field empty (robot is there :)
field[start_y][start_x] = ' '

# node class contains a copy of its field state as well as positions and previous move.
class Node:
	parent = None
	move = None
	field = None
	x = None
	y = None
	def __init__(self, field, x, y):
		self.field = field
		self.x = x
		self.y = y
		self.left = n**2 - 1

# print parent nodes first, recursively, to get in right (left->right) order
def print_path(node):
	if node.parent is None:
		print '[',
		return
	print_path(node.parent)
	print node.move,

def print_field(node):
	print_path(node)
	print ']'
	for i in range(n):
		for j in range(n):
			if node.x == j and node.y == i:
				print 'o',
				#print u'\u2603',
			else:
				print node.field[i][j],
		print
	print

# List of moves, indexed the same order Javier did in the image.
moves = [
	(-4,-1),
	(-1,-4),
	(4, -1),
	(1, -4),
	(4, 1),
	(1, 4),
	(-4, 1),
	(-1, 4)
	]

# perform an x move that's been checked to be ok
def nmove_x(node, move_x):
	xrange = [node.x, node.x + move_x]
	for i in range(min(xrange), max(xrange)+1):
		if i == node.x:
			continue
		if node.field[node.y][i] == ' ':
			return False
		node.field[node.y][i] = ' '
	node.x += move_x
	return True

# perform a y move that's been checked to be ok
def nmove_y(node, move_y):
	yrange = [node.y, node.y + move_y]
	for i in range(min(yrange), max(yrange)+1):
		if i == node.y:
			continue
		if node.field[i][node.x] == ' ':
			return False
		node.field[i][node.x] = ' '
	node.y += move_y
	return True

# spawns a child node and performs a certain move if possible
def spawn_move(node, move):
	global expands
	(move_x, move_y) = moves[move]

	# make sure that node (robot) never leaves field
	if node.x + move_x >= n or node.x + move_x < 0 or node.y + move_y >= n or node.y + move_y < 0:
		return None

	# copies whole node, refers back to its parent
	new_node = copy.deepcopy(node)
	new_node.parent = node
	new_node.move = move

	# make the longer (4 step) move before the short one
	if abs(move_y) > abs(move_x):
		if nmove_y(new_node, move_y) is False:
			return None
		if nmove_x(new_node, move_x) is False:
			return None
	else:
		if nmove_x(new_node, move_x) is False:
			return None
		if nmove_y(new_node, move_y) is False:
			return None

	# will have cleaned 5 extra tiles
	new_node.left -= abs(move_x) + abs(move_y)

	# another node will have been expanded successfully (global count)
	expands += 1

	return new_node

def dfs(node):
	# if dfs has finished
	if node.left <= target:
		# print and exit
		print_field(node)
		return True
	
	for i in range(len(moves)): # iterate through moves
		new_node = spawn_move(node, i) # attempt to perform move
		if new_node is None: # continue if unable to perform move
			continue
		if dfs(new_node): # attempt a dfs from subnode
			return True # if ok, this search was also successful
	return False

def bfs(node):
	bfsqueue = Queue.Queue()
	bfsqueue.put(node)

	while True:
		node = bfsqueue.get()
		for i in range(len(moves)): # iterate through moves, add one at a time
			new_node = spawn_move(node, i) # attempt move
			if new_node is None:
				continue #continue if unable to perform move
			if new_node.left <= target: # if done, print, finish and return True
				print_field(new_node)
				return True
			bfsqueue.put(new_node) # add new nodes to queue
		if bfsqueue.empty(): # unable to finish, no moves left
			return False

# prefer the move that leads to a 'folding' move, this was beneficial in answers found by dfs or bfs before.
def pref_next(node):
	if node.parent is None:
		return 0

	if node.move == 0:
		return 2
	if node.move == 2:
		return 0

	if node.move == 3:
		return 5
	if node.move == 5:
		return 3

	if node.move == 4:
		return 6
	if node.move == 6:
		return 4

	if node.move == 7:
		return 1
	if node.move == 1:
		return 7

	return 0

def bestfirst(node):
	if node.left <= target:
		print_field(node)
		return True

	# prefer the folding move, place this move first in the movelist
	pref_move = pref_next(node)
	movelist = list(range(len(moves)))
	movelist.remove(pref_move)
	movelist.insert(0, pref_move)

	# attempt dfs, but with this modified order instead. (Same as above!)
	for i in movelist:
		new_node = spawn_move(node, i)
		if new_node is None:
			continue
		if bestfirst(new_node):
			return True
	return False

# start node, will never be modified, just copied
first = Node(field, start_x, start_y)

# best-first search
print "best first:"
expands = 0
bestfirst(first)
print expands, 'node expansions'

# dfs
print "DFS:"
expands = 0
dfs(first)
print expands, 'node expansions'

# bfs (very slow)
print "BF.. wait for it.."
sys.stdout.flush()
expands = 0
bfs(first)
print '.. S!', expands, 'node expansions'

#done. That is All.
