#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# This is a small and inefficient program for solving hw1 partB in the course
# ai10. No effort has been made to optimize it in regards to speed or memory
# consumption.
#
# usage: ./robot.py <room size> <maximum number of dirty cells>
#
# The room size is an integer describing the length of one of the sides in the
# square room to clean.
#
# Author: Oskar Arvidsson <oskarar@kth.se>

import sys
import math
import copy

N = 7  # the order of the square room
D = 8   # the maximum number of unvisited cells left in a solution

expanded_d = 0
expanded_b = 0
expanded_f = 0

def create_room(size) :
    room = []
    for i in range(size) :
        row = []
        for j in range(size) :
            row.append(0)
        room.append(row)
    return room

def print_room(room) :
    maximum = 1
    for i in range(len(room)) :
        for j in range(len(room)) :
            x = room[i][j]
            if type(x).__name__ == "int" and x > maximum :
                maximum = x

    maxlength = int(math.log10(maximum)+2)
    strformat = "|%%%ds " %(maxlength)
    line = "-" * ((maxlength+2) * len(room) + 1)
    print(line)
    for i in range(len(room)) :
        for j in range(len(room)) :
            sys.stdout.write(strformat %(room[i][j]))
        print("|")
        print(line)

def depth_first(room, x, y, n, d) :
    if 1+5*(n-1) >= len(room)**2-d :
        return 0

    global expanded_d
    expanded_d += 1
    for move in range(8) :
        coords = get_move(move)

        filled = fill_cells(room, x, y, coords, n)

        if len(filled) == 5 :
            xp = x+coords[4][0]
            yp = y+coords[4][1]
            if depth_first(room, xp, yp, n+1, d) == 0 :
                return 0
        reset_cells(room, filled)
    return 1

def breadth_first(room, x, y, n, d) :
    global expanded_b
    queue = [(x, y, copy.deepcopy(room), n)]
    while len(queue) > 0 :
        expanded_b += 1
        x, y, orig, n = queue.pop(0)
        for move in range(8) :
            coords = get_move(move)
            state = copy.deepcopy(orig)
            if len(fill_cells(state, x, y, coords, n)) == 5 :
                if 1+5*n >= len(room)**2-d :
                    del room[:]
                    room.extend(state)
                    return 0
                queue.append((x+coords[4][0], y+coords[4][1], state, n+1))
    return 1

def best_first(room, x, y, n, d, eval_func) :
    global expanded_f
    queue = [(x, y, copy.deepcopy(room), n, eval_func(room, x, y))]
    while len(queue) > 0 :
        expanded_f += 1
        x, y, orig, n, _t = queue.pop(0)
        for move in range(8) :
            coords = get_move(move)
            state = copy.deepcopy(orig)
            if len(fill_cells(state, x, y, coords, n)) == 5 :
                if 1+5*n >= len(room)**2-d :
                    del room[:]
                    room.extend(state)
                    return 0
                xp = x+coords[4][0]
                yp = y+coords[4][1]

                k = eval_func(state, xp, yp)
                pos = 0
                for obj in queue :
                    if obj[4] > k :
                        break
                    pos += 1
                queue.insert(pos, (xp, yp, state, n+1, k))
    return 1

# Could be used as evaluation function.
def num_moves(room, x, y) :
    num = 0
    for move in range(8) :
        num += 1
        for i,j in get_move(move) :
            if x+i < 0 or x+i >= len(room) or y+j < 0 or y+j >= len(room) \
                    or room[x+i][y+j] != 0 :
                num -= 1
                break
    return num

# Could be used as evaluation function.
def num_dirty(room, x, y) :
    num = 0
    for i in [-1, 0, 1] :
        for j in [-1, 0, 1] :
            if x+i >= 0 and x+i < len(room) and y+j >= 0 and y+j < len(room) \
                    and room[x+i][y+j] == 0 :
                num += 1
    return num

def fill_cells(room, x, y, coords, n) :
    filled = []
    for i,j in coords :
        if x+i < 0 or x+i >= len(room) or y+j < 0 or y+j >= len(room) \
                or room[x+i][y+j] != 0 :
            break
        room[x+i][y+j] = n
        filled.append((x+i,y+j))
    return filled

def reset_cells(room, coords) :
    for i, j in coords :
        room[i][j] = 0

def get_move(move) :
    moves = [ \
        [(0,1), (0,2), (0,3), (0,4), (1,4)], \
        [(1,0), (2,0), (3,0), (4,0), (4,1)], \
        [(1,0), (2,0), (3,0), (4,0), (4,-1)], \
        [(0,-1), (0,-2), (0,-3), (0,-4), (1,-4)], \
        [(0,-1), (0,-2), (0,-3), (0,-4), (-1,-4)], \
        [(-1,0), (-2,0), (-3,0), (-4,0), (-4,-1)], \
        [(-1,0), (-2,0), (-3,0), (-4,0), (-4,1)], \
        [(0,1), (0,2), (0,3), (0,4), (-1,4)], \
    ]
    return moves[move]

def main(argv) :
    N = int(argv[1])
    D = int(argv[2])
    room = create_room(N)
    room[0][0] = "x"

    room_d = copy.deepcopy(room)
    if depth_first(room_d, 0, 0, 1, D) == 0 :
        print_room(room_d)
    else :
        print("no solution found with depth first search.")
    print("%d nodes were expanded with depth first search." %(expanded_d))

    room_b = copy.deepcopy(room)
    if breadth_first(room_b, 0, 0, 1, D) == 0 :
        print_room(room_b)
    else :
        print("no solution found with breadth first search.")
    print("%d nodes were expanded with breadth first search." %(expanded_b))

    evaluation_func = num_dirty
    room_f = copy.deepcopy(room)
    if best_first(room_f, 0, 0, 1, D, evaluation_func) == 0 :
        print_room(room_f)
    else :
        print("no solution found with best first search.")
    print("%d nodes were expanded with best first search." %(expanded_f))

if __name__ == "__main__" :
    if len(sys.argv) != 3 :
        print("usage: %s <room size> <maximum number of dirty cells>" \
                %(sys.argv[0]))
    else :
        main(sys.argv)

