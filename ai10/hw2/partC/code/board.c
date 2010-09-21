#include "types.h"
#include "stdio.h"
#include "board.h" 
#include <stdlib.h>
int numConflicts(board * inBoard, amazon piece[]){
	int i, j;
	int numOK = 0;
	for(i=0; i < inBoard->size; i++){
		for(j=0; j < inBoard->size; j++){
			if(inBoard->tiles[i][j] == 0x0 && (piece[j].row == i)){
				numOK++;
			}
		}
	}
	#ifdef DEBUG
	printf("Found %i without conflict and size is %i\n",numOK, inBoard->size);
	#endif
	
	return (inBoard->size - numOK);
			
}


int resetBoard(amazon piece[], int size){
	int i;
	for(i = 0; i < size; i++){
		piece[i].row = rand() % size;
	}
	return 0;
}