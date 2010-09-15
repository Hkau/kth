#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct
{
	int conflicts_x[NUM_AMAZON];
	int conflicts_y[NUM_AMAZON];

	int conflicts_diag_asc[2*NUM_AMAZON]; // "Ascending diagonal"
	int conflicts_diag_desc[2*NUM_AMAZON]; // "Descending diagonal"

	int pos_y[NUM_AMAZON];

	int conflicting_pieces;
} amazon_board;

void print_board(amazon_board *board);

inline int diag_asc_pos(int x, int y)
{
	return (NUM_AMAZON-1-y)+x;
}

inline int diag_desc_pos(int x, int y)
{
	return x+y;
}

int num_conflicts(amazon_board *board, int x, int y)
{
	int num_conflicts = board->conflicts_x[x] +
		board->conflicts_y[y] +
		board->conflicts_diag_asc[diag_asc_pos(x, y)] +
		board->conflicts_diag_desc[diag_desc_pos(x, y)];
// amazon shittier

	if(x >= 2 && board->pos_y[x-2] != -1 && (board->pos_y[x-2] == y-1 || board->pos_y[x-2] == y+1))
		++num_conflicts;

	if(x < NUM_AMAZON-2 && board->pos_y[x+2] != -1 && (board->pos_y[x+2] == y-1 || board->pos_y[x+2] == y+1))
		++num_conflicts;

	if(x >= 1 && board->pos_y[x-1] != -1 && (board->pos_y[x-1] == y-2 || board->pos_y[x-1] == y+2))
		++num_conflicts;

	if(x < NUM_AMAZON-1 && board->pos_y[x+1] != -1 && (board->pos_y[x+1] == y-2 || board->pos_y[x+1] == y+2))
		++num_conflicts;

	return num_conflicts;
}

void remove_pawn(amazon_board *board, int x)
{
	if(board->pos_y[x] == -1)
		return;

	--board->conflicts_x[x];
	--board->conflicts_y[board->pos_y[x]];

	--board->conflicts_diag_asc[diag_asc_pos(x, board->pos_y[x])];
	--board->conflicts_diag_desc[diag_desc_pos(x, board->pos_y[x])];

	if(num_conflicts(board, x, board->pos_y[x] != 0))
		--board->conflicting_pieces;

	board->pos_y[x] = -1;
}

int place_pos(amazon_board *board, int x, int y)
{
	if(board->pos_y[x] != -1)
		remove_pawn(board, x);

	if(num_conflicts(board, x, y) != 0)
		++board->conflicting_pieces;

	++board->conflicts_x[x];
	++board->conflicts_y[y];

	++board->conflicts_diag_asc[diag_asc_pos(x, y)];
	++board->conflicts_diag_desc[diag_desc_pos(x, y)];

	board->pos_y[x] = y;
}

void init_board(amazon_board *board)
{
	memset(board, 0, sizeof(amazon_board));
	int i;
	for(i = 0; i < NUM_AMAZON; ++i)
		board->pos_y[i] = -1;

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


	for(i = 0; i < NUM_AMAZON; ++i)
	{
		place_pos(board, i, placements[i]);
	}
}

bool move_is_valid(amazon_board *board, int x, int y)
{
	return num_conflicts(board, x, y) == 0;
}

bool solve(amazon_board *board)
{
	int solve_iter = 0;
	if(NUM_AMAZON != 1 && NUM_AMAZON < 5)
		return false;
	int iter = 0;
	while(true)
	{
		
		//print_board(board);
		++iter;
		// todo, try random index order
		int i;
		int start_idx = rand() % NUM_AMAZON;
		int max_x = start_idx, max_obst = num_conflicts(board, start_idx, board->pos_y[start_idx]);
		for(i = start_idx+1; i != start_idx; ++i)
		{
			if(i == NUM_AMAZON)
			{
				i = 0;
				if(start_idx == 0)
					break;
			}

			int obst = num_conflicts(board, i, board->pos_y[i]);

			if(obst > max_obst)
			{
				max_obst = obst;
				max_x = i;
			}
		}

		// remove pos from board so that it doesn't interfere with readings
		//remove_pawn(board, max_x); // though it seems non-beneficial
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

		place_pos(board, max_x, min_y);

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

		if(++solve_iter == NUM_AMAZON) // good limit? I think not?
		{
			solve_iter = 0;
			init_board(board);
		}
	}
}

int main(int argc, char *argv[])
{
	printf("num_amazons: %d sizeof: %d\n", NUM_AMAZON, sizeof(amazon_board));

	amazon_board board;
	init_board(&board);

	if(!solve(&board))
	{
		puts("det blev ingen cd. :(");
		return 0;
	}
	// print_board(&board);

	return 0;
}

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

