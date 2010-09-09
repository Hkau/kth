#!/usr/bin/python

import sys
import os.path

def printElevatorCommands(commandList):
    if commandList:
        print "Your commands for the first elevator agent are: "
        for comm in commandList:
            if comm==-1:
                print "down",
            elif comm==0:
                print "stay",
            elif comm==1:
                print "up",
            else:
                print "Only commands [-1,0,1] are allowed"
        print "\n\n"
    else:
        print "Your command list was empty\n\n"

def searchInformation(searchInfoList):
    methods=['bfs','dfs','ids']
    if len(searchInfoList) != 3:
        "Your solution for each part of exercise 3 should contain 3 lists,\
                corresponding to bfs, dfs and ids search methods"
    else:
        for methodIndex in range(len(searchInfoList)):
            if len(searchInfoList[methodIndex])!=3:
                print "Problem in method %s" % methods[methodIndex]
                print "Your solution for each method in exercise 3 should contain 3 elements:\n\
                - an integer for the worst case time spent\n\
                - and integer for the worst case maximum memory consumption\n\
                - a list of nodes with the nodes in memory when the solution is found (empty if the max memory consumption didn't fit in memory)\n"
            else:
                print "Worst case for %s method took %s microseconds and its maximum memory consumption is %s memory nodes." % \
                        (methods[methodIndex], searchInfoList[methodIndex][0],searchInfoList[methodIndex][1])
                if searchInfoList[methodIndex][2]:
                    print " The nodes in memory when the solution was found were: "  + ', '.join(map(lambda x:str(x),searchInfoList[methodIndex][2]))
                else:
                    print " The search didn't fit in memory"



if os.path.basename(sys.argv[1]) == 'answers.txt':
    print "Please rename your answers.txt to answers.py for this test. Thanks"
    sys.exit(-1)
elif os.path.basename(sys.argv[1]) != 'answers.py':
    print "Please call your answers file answers.py. Thanks"
    sys.exit(-1)

sys.path.append(os.path.dirname(sys.argv[1]))
import answers

answerVars   = dir(answers)
expectedVars = set(['ex1','ex2a','ex2b','ex3a','ex3b'])
if expectedVars.difference(answerVars):
    for var in expectedVars.difference(answerVars):
        print "Variable %s is not present in your answers.py file. Didn't you answer the question or did you named it differently?" % var#\
        #If you haven't answer %s, leave it empty (i.e. %s=[])" % (var,var,var)

if 'ex1' in answerVars:
    print " === Exercise 1 ==="
    if len(answers.ex1)!=7:
        print "If you don't want to answer one question in the test, please write -1 on it"
    else:
        if 3 in answers.ex1:
            print "Answers should be ordered as in the description of the exercise, from 0 to 2"
            sys.exit(-1)
        print "Exercise 1 answers are: "
        answersOrdinals=['first','second','third']
        for index in range(len(answers.ex1)):
            print "\t The correct answer for question %d is the %s answer (number %d); " % (index, answersOrdinals[answers.ex1[index]],answers.ex1[index])
        print "\n"

if 'ex2a' in answerVars:
    print " === Exercise 2a ==="
    printElevatorCommands(answers.ex2a)
if 'ex2b' in answerVars:
    print " === Exercise 2b ==="
    printElevatorCommands(answers.ex2b)

if 'ex3a' in answerVars:
    print " === Exercise 3a ==="
    searchInformation(answers.ex3a)
if 'ex3b' in answerVars:
    print " === Exercise 3b ==="
    searchInformation(answers.ex3b)
