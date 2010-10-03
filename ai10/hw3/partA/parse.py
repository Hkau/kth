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
expectedVars = set(['ex1','ex2a','ex2b','ex2c','ex3a','ex3b'])
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
    try:
        if len(answers.ex2a)!=1:
            if answers.ex2a:
                print('Your ex2a answer %s should be a list with one element' % str(answers.ex2a))
            else:
                print('Your ex2a answer is empty')
        else:
            print('The probability of CANDIDATE X WINS is %0.3f' % answers.ex2a[0])
    except:
        print('Your ex2a answer %s is probably a number instead of a list of a number [number]' % str(answers.ex2a))
if 'ex2b' in answerVars:
    print( " === Exercise 2b ===")
    try:
        if len(answers.ex2b)!=1:
            if answers.ex2b:
                print('Your ex2b answer %s should be a list with one element' % str(answers.ex2b))
            else:
                print('Your ex2b answer is empty')
        else:
            print('The maximum cost of buying the vote is %0.3f' % answers.ex2b[0])
    except:
        print('Your ex2b answer %s is probably a number instead of a list of a number [number]' % str(answers.ex2b))
if 'ex2c' in answerVars:
    print( " === Exercise 2c ===")
    if len(answers.ex2c)!=3:
        if answers.ex2c:
            print('Your answers.ex2c answer %s should be a list with three elements' % str(answers.ex2c))
        else:
            print('Your ex2c answer is empty')
    else:
        print('The probabilities of nodes being false given CANDIDATE X WINS is false are %s' % ','.join(map(lambda x:'%0.3f'%x,answers.ex2c)))

if 'ex3a' in answerVars:
    print( " === Exercise 3a ===")
    if answers.ex3a:
        if len(answers.ex3a)!=3:
            print('Your answers.ex3a answer %s should be a list with three elements' % str(answers.ex3a))
        else:
            try:
                for key in answers.ex3a.keys():
                    print('The utility of node %d is %0.3f' % (key,answers.ex3a[key]))
            except:
                print('I could not acces the variables in your ex3a answer %s; did you remember to put them into curly brackets{} and following the format NODENUMBER:UTILITY?' % str(answers.ex3a))
    else:
        print( 'Your answer ex3a is empty')
if 'ex3b' in answerVars:
    print( " === Exercise 3a ===")
    if answers.ex3b:
        if len(answers.ex3b)!=3:
            print('Your answers.ex3b answer %s should be a list with three elements' % str(answers.ex3b))
        else:
            try:
                for key in answers.ex3b.keys():
                    print('The utility of node %d is %0.3f' % (key,answers.ex3b[key]))
            except:
                print('I could not acces the variables in your ex3b answer %s; did you remember to put them into curly brackets{} and following the format NODENUMBER:UTILITY?' % str(answers.ex3b))
    else:
        print( 'Your answer ex3b is empty')

