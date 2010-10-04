#value-iteration

reward = [-0.01, 1, -1, -0.01, -0.01]
discount = 0.9
iterations = 5

def utility(nodes, i):
	if i == 0:
		return max(
			nodes[3]*0.9+nodes[0]*0.1, # blue path incl. overshoot
			nodes[1]*0.9+nodes[2]*0.1 # orange path incl. overshoot
			)
	if i == 1:
		return 0 # terminal path, do not update
	if i == 2:
		return 0 # terminal path, do not update
	if i == 3:
		return max(
			nodes[0]*0.9+nodes[3]*0.1, # blue
			nodes[4]*0.9+nodes[0]*0.1 # orange
			)
	if i == 4:
		return max(
			nodes[2]*0.9+nodes[4]*0.1, # blue
			nodes[0]*0.9+nodes[1]*0.1 # orange
			)


U = [0, reward[1], reward[2], 0, 0]

print 'discount::', discount
for i in range(1, iterations+1):
	tmp = []
	for j in range(5): # non-terminal nodes
		tmp.append(reward[j] + discount*utility(U, j))

	U = tmp #copy.deepcopy(tmp)
	print i, ':', U

#for j in [1, 2]:
#	U[j] = reward[j]

print U
