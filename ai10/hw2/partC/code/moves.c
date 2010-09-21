#include "types.h"
#include "moves.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
int paintDirty(board * inBoard, amazon piece[]){
	int size = inBoard->size;
	int i, j;
	/*reset all tiles and fill in the pieces */
	for(i=0; i<size; i++){
		for(j=0; j<size; j++){
			inBoard->tiles[i][j] = 0;
		}
	}
	for(i=0;i<size;i++){
		rook(inBoard, i, piece[i].row);
		knight(inBoard, i, piece[i].row);
		bishop(inBoard, i, piece[i].row);
	}
	
	return 0;
}

/*prints supplied board */
int printBoard(board * inBoard, amazon piece[]){

	int i, j;
	int size = inBoard->size;
	for(i=0; i<size; i++){
		printf("\nRow %i\t",i);
		for(j=0; j<size; j++){
			/* piece in column j at row i*/
			if(piece[j].row == i){
				printf("%c%c ",0x40+inBoard->tiles[i][j], 0x40+inBoard->tiles[i][j]);
			} else { /*no piece */
				printf("%02i ",inBoard->tiles[i][j]);
			}
			
		}
	}
	printf("\n");
	return 0;
}

/*prints raw numbers of board, used for debugging purposes only*/
int printBoardRAW(board * inBoard, amazon piece[]){
	int i, j;
	int size = inBoard->size;
	for(i=0; i<size; i++){
		printf("\nRow %i\t",i);
		for(j=0; j<size; j++){
			printf("%02x  ",inBoard->tiles[i][j]);
		}
	}
	printf("\n");
	return 0;
}

/* looks through the board and notes the number of conflicts for each piece*/
int populateConflictList(board * inBoard, amazon piece[], int conflictList[]){
	int i, j;
	for(i=0;i<inBoard->size;i++){
		conflictList[i] = 0;
	}
	for(i=0; i< inBoard->size; i++){
		for(j=0; j< inBoard->size; j++){
			if(inBoard->tiles[i][j] > 0 && piece[j].row == i){
				conflictList[j] = inBoard->tiles[i][j];
			}
		}
	}
	return 0;
}

int rook(board * inBoard, int x, int y){
	int i;
	for(i = 0;i < inBoard->size;i++){
		inBoard->tiles[y][i] += 1;
	}
	inBoard->tiles[y][x] -= 1;/*don't conflcit with self*/
	return 0;
}

/* BROKEN*/
/* TODO: FIX IT!*/
int bishop(board * inBoard, int x, int y){
	int i, ii;
	int size = inBoard->size;

	for (i=x+1, ii=y+1; i < size 	&& ii < size; 	i++, ii++){
		inBoard->tiles[ii][i] += 1;
	}
	for (i=x+1, ii=y-1; i < size 	&& ii >= 0; 		i++, ii--){
		inBoard->tiles[ii][i] += 1;
	}
	for (i=x-1, ii=y-1; i >= 0 		&& ii >= 0; 		i--, ii--){
		inBoard->tiles[ii][i] += 1;
	}
	for (i=x-1, ii=y+1; i >= 0 		&& ii < size; 	i--, ii++){
		inBoard->tiles[ii][i] += 1;
	}
	return 0;
}


int knight(board * inBoard, int x, int y){
	int size = inBoard->size;
	int d[4] = {1,-1,2,-2};
	int i;
	int j;
	int dx, dy; /*only used to make inner if() readable*/
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(abs(d[i]) != abs(d[j])){
				dy = d[j] + y;
				dx = d[i] + x;
				if(dx >= 0 && dx < size && dy >= 0 && dy < size){
					inBoard->tiles[y+d[j]][x+d[i]] += 1;
				}
			}
		}
	}
		
	return 0;
}

/* picks a random piece that is in conflict and moves it to a min-conflict position
 * also resets the waiting-counter for that piece */
int pickNextandMove(amazon piece[],int conflictList[], int nConflicts, board * inBoard){
	int i = 0;
	int j = 0;
	int steps;
	int pToMove = 0;
	int min = INT_MAX;
	steps = rand() % nConflicts;
	#ifdef DEBUG
	printf("trying to move the %i:th piece in conflict\n", steps);
	#endif
	while(i<steps){
		/*Don't count empty posts in conflictList when stepping through*/
		if(conflictList[j] > 0){
			pToMove= j;
			i++;
		}
		j++;
	}
	/*find the tile woth the least conflict in this column*/
	/*this will break for unresonably small boards*/
	for(i=0;i<inBoard->size;i++){
		if(inBoard->tiles[i][pToMove] < min){
			min = inBoard->tiles[i][pToMove];
			piece[pToMove].row = i;
			
		}
	}
	piece[pToMove].waiting = 0;
	#ifdef DEBUG
	printf("moved piece %i to row %i\n", pToMove, piece[pToMove].row);
	#endif
	return 0;
}

int pickNextandMoveNORAND(amazon piece[],int conflictList[], int nConflicts, board * inBoard){
	int i;
	int pToMove = -1;
	int max = 0;
	int min = INT_MAX;
	for(i=0; i<inBoard->size; i++){
		if(conflictList[i] > max && piece[i].waiting > 1){
			max = conflictList[i];
			pToMove = i;
		}
	}
	
	/*find the tile with the least conflict in this column*/
	for(i=0;i<inBoard->size;i++){
		if(inBoard->tiles[i][pToMove] < min){
			min = inBoard->tiles[i][pToMove];
			piece[pToMove].row = i;
			
		}
	}
	piece[pToMove].waiting = 0;
	#ifdef DEBUG
	printf("moved piece %i to row %i\n", pToMove, piece[pToMove].row);
	#endif
	return 0;
}

/*counts the number of iterations a piece has been standing still*/
int agePieces(amazon piece[],int size){
	int i;
	for(i=0;i<size;i++){
		piece[i].waiting +=1;
	}
	return 0;
}