states = ('RIGHT', 'LEFT', 'STOP', 'FORWARD')
 
observations = ('H1', 'H3', 'H3', 'H4', 'H2', 'H1', 'H3', 'H1', 'H1', 'H2')
 
start_probability = {'RIGHT': 1, 'LEFT':0, 'STOP': 0, 'FORWARD':0}
 
transition_probability = {
   'RIGHT' : {'RIGHT': 0.45, 'LEFT':0.09, 'STOP': 0.10, 'FORWARD': 0.20},
   'LEFT' : {'RIGHT': 0.08, 'LEFT':0.55, 'STOP': 0.10, 'FORWARD': 0.27},
   'STOP' : {'RIGHT': 0.08, 'LEFT':0.12, 'STOP':0.60, 'FORWARD':0.20},
   'FORWARD' : {'RIGHT': 0.09, 'LEFT':0.09, 'STOP':0.15, 'FORWARD':0.67},
   }
 
emission_probability = {
   'RIGHT' : {'H1':0.60, 'H2':0.05, 'H3':0.15, 'H4':0.20},
   'LEFT' : {'H1':0.12, 'H2':0.62, 'H3':0.10, 'H4':0.16},
   'STOP' : {'H1':0.15, 'H2':0.20, 'H3':0.63, 'H4':0.02},
   'FORWARD' : {'H1':0.01, 'H2':0.09, 'H3':0.25, 'H4':0.70},
   }

# Helps visualize the steps of Viterbi.
def print_dptable(V):
    print "    ",
    for i in range(len(V)): print "%7s" % ("%d" % i),
    print
 
    for y in V[0].keys():
        print "%.5s: " % y,
        for t in range(len(V)):
            print "%.7s" % ("%f" % V[t][y]),
        print
 
def viterbi(obs, states, start_p, trans_p, emit_p):
    V = [{}]
    path = {}
 
    # Initialize base cases (t == 0)
    for y in states:
        V[0][y] = start_p[y] * emit_p[y][obs[0]]
        path[y] = [y]
 
    # Run Viterbi for t > 0
    for t in range(1,len(obs)):
        V.append({})
        newpath = {}
 
        for y in states:
            (prob, state) = max([(V[t-1][y0] * trans_p[y0][y] * emit_p[y][obs[t]], y0) for y0 in states])
            V[t][y] = prob
            newpath[y] = path[state] + [y]
 
        # Don't need to remember the old paths
        path = newpath
 
    print_dptable(V)
    (prob, state) = max([(V[len(obs) - 1][y], y) for y in states])
    return (prob, path[state])


def example():
    return viterbi(observations,
                   states,
                   start_probability,
                   transition_probability,
                   emission_probability)
print "that is"
print example()


