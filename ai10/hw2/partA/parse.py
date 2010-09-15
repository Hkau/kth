#!/usr/bin/python
# Execute python parse.py answers.py to see if answers.py is compliant

import sys
import os.path

def displayAssignedVariables(assignments):
    if assignments:
        for i in range(len(assignments)):
            print('=Assignment %s='%i)
            for var,val in assignments[i].items():
                print('Variable %s is assigned value %s'%(var,val))
    else:
        print('Your assignments are empty')

if os.path.basename(sys.argv[1]) == 'answers.txt':
    print( "Please rename your answers.txt to answers.py for this test. Thanks")
    sys.exit(-1)
elif os.path.basename(sys.argv[1]) != 'answers.py':
    print( "Please call your answers file answers.py. Thanks")
    sys.exit(-1)

sys.path.append(os.path.dirname(sys.argv[1]))
import answers

answerVars   = dir(answers)
expectedVars = set(['ex1','ex2a','ex2b','ex3a','ex3b'])
if expectedVars.difference(answerVars):
    for var in expectedVars.difference(answerVars):
        print( "Variable %s is not present in your answers.py file. Didn't you answer the question or did you named it differently?" % var)#\
        #If you haven't answer %s, leave it empty (i.e. %s=[])" % (var,var,var)

if 'ex1' in answerVars:
    print( " === Exercise 1 ===")
    if len(answers.ex1)!=7:
        print( "If you don't want to answer one question in the test, please write -1 on it")
    else:
        if 3 in answers.ex1:
            print( "Answers should be ordered as in the description of the exercise, from 0 to 2")
            sys.exit(-1)
        print( "Exercise 1 answers are: ")
        answersOrdinals=['first','second','third']
        for index in range(len(answers.ex1)):
            print( "\t The correct answer for question %d is the %s answer (number %d); " % (index, answersOrdinals[answers.ex1[index]],answers.ex1[index]))
        print( "\n")

if 'ex2a' in answerVars:
    print( " === Exercise 2a ===")
    displayAssignedVariables(answers.ex2a)
if 'ex2b' in answerVars:
    print( " === Exercise 2b ===")
    displayAssignedVariables(answers.ex2b)

if 'ex3a' in answerVars:
    print( " === Exercise 3a ===")
    if answers.ex3a:
        print( ' MAX player should choose action %s' % answers.ex3a)
    else:
        print( 'Your answer for MAX player action is empty')

if 'ex3b' in answerVars:
    print( " === Exercise 3b ===")
    if answers.ex3b:
        print( ' Nodes %s should be pruned by the alpha-beta pruning' % ', '.join(answers.ex3b))
    else:
        print( 'Your answer about which nodes were pruned is empty')
