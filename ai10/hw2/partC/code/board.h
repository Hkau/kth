/* functions for handling the board
 * numConflicts() returns the total number of pieces in conflict on the board
 * if 0, the problem is solved(no piece in conflict with another)
 *
 * resetBoard() places the provided pieces in random positions 
 *
 * TODO: make sure the functions in move.c and board.c are placed more logically, messy now
 * TODO: write descriptions of functions
 */

#ifndef board_h
#define board_h
#include "types.h"

int numConflicts(board * inBoard, amazon piece[]);
int resetBoard(amazon piece[], int size);

#endif