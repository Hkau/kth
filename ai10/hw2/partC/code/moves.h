/* functions for calculating moves, printing board to screen and similar stuff
 *
 * paintDirty() calculates all the tiles that become "dirty/conflicted" from the array of pieces
 * This is additive, so two pieces covering the same tile
 * makes the tile "dirtier", eg. it has more conflicts
 *
 * printBoard() prints the whole board, with numbers for conflicty tiles. this is only reasonable
 * for smaller boards as every piece is marked with( @ + conflictyness of tile)
 * meaning a piece standing on a tile with conflicty = 3 will have the value (@ = 0x40):
 * 0x40 + 0x03 = 0x43 = 'C'. Thus, if a tile is conflictier than 26 it is difficult to keep track
 * of the character values. There will also be problems when the conflicty value comes close to
 * the limit of char-type. Only use it for printing solutions with larger boards and hope
 * your terminal is wide enough
 *
 * queen() used by paintDirty to calculate the queens movement
 * bishop() and knight() is the same idea as queen()
 *
 * pickNextAndMove() takes one piece that is in conflict/standing on a dirty tile and
 * moves it. the piece is chosen randomly from a list of only pieces in conflict. 
 *
 * agePieces() adds to the counter of how many iterations a piece has been immobile
 * 
 * TODO: fix bishop(), it is broken now
 */


#ifndef moves_h
#define moves_h

#include "types.h"
 int paintDirty(board * inBoard, amazon piece[]);
 int printBoard(board * inBoard, amazon piece[]);
 int printBoardRAW(board * inBoard, amazon piece[]);
 int rook(board * inBoard, int x, int y);
 int bishop(board * inBoard, int x, int y);
 int knight(board * inBoard, int x, int y);
 int populateConflictList(board * inBoard, amazon piece[], int * conflictList);
 int pickNextandMove(amazon piece[],int conflictList[], int nConflicts, board * inBoard);
 int pickNextandMoveNORAND(amazon piece[],int conflictList[], int nConflicts, board * inBoard);
 int agePieces(amazon piece[],int size);
#endif