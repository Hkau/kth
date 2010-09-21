#ifndef types_h
#define types_h

/*struct for storing the board together with boardsize, for convenience
 * TODO: maybe include amazon-array in struct, to make passing arguments "cleaner"(less of them to pass around, lot of rewrites to be done though.... )
 */
typedef struct{
	int ** tiles;
	int size;
} board;

typedef struct{
	int row;
	int waiting; /*keeps track of how long the piece has been immobile*/
} amazon;

#endif