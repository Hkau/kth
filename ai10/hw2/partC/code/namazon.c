/* solves the n-amzon problem
 * amazon = chess piece with a movement pattern a combination of a queen and knight
 * or rook + bishop + knight(as it is implemented here)
 * supply size of board as command line argument
 * TODO: make sure arguments supplied are reasonable(not silly small sizes, or negative)
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "types.h"
#include "moves.h"
#include "board.h"

int main(int argc, char * argv[]){
	int size;
	int i = 0; 
	int j = 0;
	int k = 0;
	int oldestPiece;
	int maxAge;
	int iterations = 0; /*used to keep track of total number of iterations for solution*/
	amazon * piece;	/*array of all the pieces */
	int * conflictList; /*list of all pieces in conflict*/
	int nConflicts= -1; /*total no of conflicts on board*/
	board * currBoard = malloc(sizeof(board)); /* struct that describes board state*/
	
	srand(time(0)); /*init with 'true' random seed for starting board and move order*/
	
	/*check arguments if OK, if so, allocate mem for board and piecelist */
	if(argc == 2){
		size = atoi(argv[1]);
		currBoard->tiles = calloc(size*size, sizeof(int*));
		piece = calloc(size,sizeof(amazon));
		conflictList = calloc(size, sizeof(int));
		/* allocates the rows of the board and places the pieces randomly(resetBoard())*/
		for(i = 0; i < size; i++){
			currBoard->tiles[i] = calloc(size, sizeof(int));
			piece[i].row = i;
		}
		
		/*resetBoard(piece,size);*/ /*sets board randomly... not a good starting guess*/
		currBoard->size = size;
	}else {
		printf("Usage: namazon [boardsize]\n");
		return 0;
	}
	
	/*calculate the number of conflicts("dirtyness") at each tile*/
	paintDirty(currBoard, piece);
	/*don't print unreasonably large boards*/
	(size<26) ? printBoard(currBoard, piece): printf("Silent mode, board too big!\n");
	
	/*calculates the number of remaining conflits on board, if 0 returned, problem solved*/
	nConflicts = numConflicts(currBoard,piece);
	while(nConflicts > 0){
		/* j for n of iterations for current board, iterations for total n of iterations*/
		j++;
		iterations++;

		/* 
		 * if some pieces are stuck for too long, move them around a bit to mix things up
		 */		
		if(j> size/5){ 
		for(k=0;k<size/100 +1;k++){ /*amount of pieces to throw around*/
			oldestPiece = -1;
			maxAge = 0;
			/*resetBoard(piece, size);*//*expensive to do....*/
			for(i=0;i<size;i++){ 
				if(piece[i].waiting > maxAge){
					oldestPiece = i;
					maxAge = piece[oldestPiece].waiting;
				}
			}
			piece[oldestPiece].row = rand() % size;
			piece[oldestPiece].waiting = 0;
		}
			j = 0;
		}
		
		/*puts the number of conflicts for each piece in an array*/
		populateConflictList(currBoard,piece, conflictList);
		
		#ifdef DEBUG
		printf("conflictList: ");
		for(i=0; i<size;i++){
			printf("%x ",conflictList[i]);
		}
		printf("\n");
		#endif
		
		/*pick next piece to move from conflictList*/
		agePieces(piece,size);
		pickNextandMove(piece,conflictList, nConflicts, currBoard);
		/*pickNextandMoveNORAND(piece,conflictList, nConflicts, currBoard);*/
		paintDirty(currBoard, piece);
		nConflicts = numConflicts(currBoard,piece);
		
		
		#ifdef DEBUG
		printBoard(currBoard, piece);
		sleep(1);
		#endif
	
	}
	
	paintDirty(currBoard, piece);
	(size<26) ? printBoard(currBoard, piece): printf("Silent mode, board too big!\n");
	printf("\nFound solution to board of size %i within %i iterations\n",size,iterations);
	return 0;
}