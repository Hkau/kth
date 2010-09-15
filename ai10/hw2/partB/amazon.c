#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct
{
	// conflicts in columns, each element will, because of the implementation, however not exceed 1, old relic..
	int conflicts_x[NUM_AMAZON];
	// number of row conflicts (amazons placed in a row).
	int conflicts_y[NUM_AMAZON];

	// number of diagonal conflicts (num of amazons placed in diagonal).
	int conflicts_diag_asc[2*NUM_AMAZON]; // "Ascending diagonal"
	int conflicts_diag_desc[2*NUM_AMAZON]; // "Descending diagonal"

	// y-pos corresponding to its index
	int pos_y[NUM_AMAZON];

	// old relic /unused
	//int conflicting_pieces;
} amazon_board;

void print_board(amazon_board *board);

// calculates the ascending diagonal index for a given point
inline int diag_asc_pos(int x, int y)
{
	return (NUM_AMAZON-1-y)+x;
}

// calculates the descending diagonal index for a given point
inline int diag_desc_pos(int x, int y)
{
	return x+y;
}

// counts number of conflicts in a given board for a given point
int num_conflicts(amazon_board *board, int x, int y)
{
	// First, sum the number of row/col/diag conflicts on this point. This will include the piece's own placement as a position.
	// This is not an issue and prevents the board from being placed at the same spot it's taken from. 
	int num_conflicts = board->conflicts_x[x] +
		board->conflicts_y[y] +
		board->conflicts_diag_asc[diag_asc_pos(x, y)] +
		board->conflicts_diag_desc[diag_desc_pos(x, y)];
// amazon shittier

	// add knight move conflicts for each possible position if there's an amazon placed in those spots. 
	// also does out of bounds checks, these checks are not cached and do not need to be updated.
	if(x >= 2 && board->pos_y[x-2] != -1 && (board->pos_y[x-2] == y-1 || board->pos_y[x-2] == y+1))
		++num_conflicts;

	if(x < NUM_AMAZON-2 && board->pos_y[x+2] != -1 && (board->pos_y[x+2] == y-1 || board->pos_y[x+2] == y+1))
		++num_conflicts;

	if(x >= 1 && board->pos_y[x-1] != -1 && (board->pos_y[x-1] == y-2 || board->pos_y[x-1] == y+2))
		++num_conflicts;

	if(x < NUM_AMAZON-1 && board->pos_y[x+1] != -1 && (board->pos_y[x+1] == y-2 || board->pos_y[x+1] == y+2))
		++num_conflicts;

	// count is now correct, return
	return num_conflicts;
}

void remove_pawn(amazon_board *board, int x)
{
	// do not remove an amazon where one was not placed in the first place (-1 reserved for unplaced), should not happen.
	if(board->pos_y[x] == -1)
		return;

	// update row, col, diag conflicts
	--board->conflicts_x[x];
	--board->conflicts_y[board->pos_y[x]];

	--board->conflicts_diag_asc[diag_asc_pos(x, board->pos_y[x])];
	--board->conflicts_diag_desc[diag_desc_pos(x, board->pos_y[x])];

	// remove placed position
	board->pos_y[x] = -1;
}

int place_pos(amazon_board *board, int x, int y)
{
	// if piece was placed, remove (-1 == unplaced)
	if(board->pos_y[x] != -1)
		remove_pawn(board, x);

	// update row, col, diag conflicts
	++board->conflicts_x[x];
	++board->conflicts_y[y];

	++board->conflicts_diag_asc[diag_asc_pos(x, y)];
	++board->conflicts_diag_desc[diag_desc_pos(x, y)];

	// set xpos' corresponding y index
	board->pos_y[x] = y;
}

void init_board(amazon_board *board)
{
	memset(board, 0, sizeof(amazon_board));

	// each piece start unplaced (idx = -1)
	int i;
	for(i = 0; i < NUM_AMAZON; ++i)
		board->pos_y[i] = -1;

	// shuffle array of possible position, random = good.
	int placements[NUM_AMAZON];
	for(i = 0; i < NUM_AMAZON; ++i)
		placements[i] = i;
	int last_idx = NUM_AMAZON -1;
	while(last_idx > 0)
	{
		int tmp_idx = rand() % (last_idx+1);
		int tmp = placements[tmp_idx];
		placements[tmp_idx] = placements[last_idx];
		placements[last_idx] = tmp;
		--last_idx;
	}

	// array shuffled, now place the pieces
	for(i = 0; i < NUM_AMAZON; ++i)
	{
		place_pos(board, i, placements[i]);
	}
}

bool solve(amazon_board *board)
{
	int solve_iter = 0;
	// if the board can't be solved, return false (boards below 10 can't be solved, except for the trivial one)
	if(NUM_AMAZON != 1 && NUM_AMAZON < 10)
		return false;
	// iteration count
	int iter = 0;
	// let's go!
	while(true)
	{
		++iter;

		int i;
		// start at a random index, this means that the first found largest becomes our candidate for movement, and not the first one by index.
		int start_idx = rand() % NUM_AMAZON;
		// current maximum, with correct number of conflicts / obstructions.
		int max_x = start_idx, max_obst = num_conflicts(board, start_idx, board->pos_y[start_idx]);
		for(i = start_idx+1; i != start_idx; ++i)
		{
			// make sure it wraps around
			if(i == NUM_AMAZON)
			{
				i = 0;
				// make sure it finishes if random idx was 0
				if(start_idx == 0)
					break;
			}

			// find index with largest number of conflicts

			int obst = num_conflicts(board, i, board->pos_y[i]);

			if(obst > max_obst)
			{
				max_obst = obst;
				max_x = i;
			}
		}

		// same as above for y position, starts with random as well, but finds minimum number of conflicts (best position)
		start_idx = rand() % NUM_AMAZON;
		int min_y = start_idx, min_obst = num_conflicts(board, max_x, start_idx);
		for(i = start_idx+1; i != start_idx; ++i)
		{
			if(i == NUM_AMAZON)
			{
				i = 0;
				if(start_idx == 0)
					break;
			}

			int obst = num_conflicts(board, max_x, i);

			if(obst < min_obst)
			{
				min_obst = obst;
				min_y = i;
			}
		}

		// place (move) current piece to its new y position
		place_pos(board, max_x, min_y);

		// if this no longer has any conflicts (except its own = 4), print iterations and return (solved!)
		int z;
		for(z = 0; z < NUM_AMAZON; ++z)
		{
			if(num_conflicts(board, z, board->pos_y[z]) != 4)
				break;
		}
		if(z == NUM_AMAZON)
		{
			printf("%d solve iterations\n", iter);
			return true;
		}

		// make sure it doesn't get stuck, each "should" be solved well within n iterations.
		if(++solve_iter == NUM_AMAZON) // good limit? I think not?
		{
			solve_iter = 0;
			init_board(board);
		}
	}
}

int main(int argc, char *argv[])
{
	printf("%d amazons\n", NUM_AMAZON);

	amazon_board board;
	init_board(&board);

	if(!solve(&board))
	{
		puts("det blev ingen cd. :(");
		return 0;
	}

	// -- uncomment to print solution!
	//print_board(&board);

	return 0;
}

// print board,
void print_board(amazon_board *board)
{
	int i;
	char *buffer[NUM_AMAZON];
	for(i = 0; i < NUM_AMAZON; ++i)
		buffer[i] = malloc(sizeof(char) * NUM_AMAZON);

	for(i = 0; i < NUM_AMAZON; ++i)
		memset(buffer[i], '-', sizeof(char)*NUM_AMAZON);

	for(i = 0; i < NUM_AMAZON; ++i)
	{
		if(board->pos_y[i] == -1)
			continue;
		buffer[i][board->pos_y[i]] = '#';
	}

	// print board
	for(i = 0; i < NUM_AMAZON; ++i)
	{
		fwrite(buffer[i], 1, sizeof(char)*NUM_AMAZON, stdout);
		printf("| %d\n", board->conflicts_x[i]);
	}

	for(i = 0; i < NUM_AMAZON; ++i)
		putchar('_');
	putchar('\n');
	for(i = 0; i < NUM_AMAZON; ++i)
		printf("%d", board->conflicts_y[i]);
	putchar('\n');

	for(i = 0; i < NUM_AMAZON; ++i)
		free(buffer[i]);
}

