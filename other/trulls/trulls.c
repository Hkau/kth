#include <stdio.h>
#include <stdlib.h>

int n;
int squares;
int *board;

struct
{
	int i, j;
} moves[] = {{0, -3}, {0, 3}, {-3, 0}, {3, 0},
	{-2, -2}, {-2, 2}, {2, -2}, {2, 2}};
// These moves are |3| horizontally and vertically and |2| diagonally.

int num_moves = sizeof(moves)/sizeof(moves[0]);

int solutions = 0;

int max_depth = 0;

unsigned long total_iter = 0;

int reachable(int i, int j)
{
	int move;
	for(move = 0; move < num_moves; ++move)
	{
		int new_i = i + moves[move].i;
		int new_j = j + moves[move].j;

		if(new_i < 0 || new_j < 0 || new_i >= n || new_j >= n)
			continue; // can't be reachable from outside board
		if(board[new_i * n + new_j] == 0)
			return 1; // found other position from where it can be reached
	}
	return 0;
}

unsigned long iter_report = 1024;
void search(int i, int j, int iter)
{
	if(++total_iter == iter_report)
	{
		printf("%ld iterations\n", total_iter);
		fflush(stdout);
		iter_report <<= 1;
	}
	if(iter > max_depth)
	{
		max_depth = iter;
		printf("max_depth: %d\n", max_depth);

		int a;
		for(a = 0; a < n; ++a)
		{
			int b;
			for(b = 0; b < n; ++b)
				printf("(%4d)", board[a*n+b]);
			putchar('\n');
		}
		fflush(stdout);
	}

	iter++;

	int move;
	for(move = 0; move < num_moves; ++move)
	{
		int new_i = i + moves[move].i;
		int new_j = j + moves[move].j;

		if(new_i < 0 || new_j < 0 || new_i >= n || new_j >= n)
			continue; // new move moves outside board, skip
		if(board[new_i * n + new_j] != 0)
			continue; // spot already taken

		board[new_i*n+new_j] = iter;

		if(iter == squares)
		{
			++solutions;
			printf("solution %d:\n", solutions);
	
			// print results
			int a;
			for(a = 0; a < n; ++a)
			{
				int b;
				for(b = 0; b < n; ++b)
					printf("(%4d)", board[a*n+b]);
				putchar('\n');
			}
		}
		else
		{
			int invalid_move = 0;
			int check; // by doing this move, are any squares that was reachable from the last board left behind?

			// note: this works because we're checking for a solution, not what's closest to a solution
			for(check = 0; check < num_moves; ++check)
			{
				int check_i = i + moves[check].i;
				int check_j = j + moves[check].j;

				if(check_i < 0 || check_j < 0 || check_i >= n || check_j >= n)
					continue; // new move moves outside board, doesn't need to be reachable
				if(board[check_i * n + check_j] != 0)
					continue; // piece already visited, doesn't need to be reachable

				if(!reachable(check_i, check_j))
				{
/*					printf("invalid (%d, %d)\n", check_i, check_j);
					int a;
					for(a = 0; a < n; ++a)
					{
						int b;
						for(b = 0; b < n; ++b)
							printf("(%4d)", board[a*n+b]);
						putchar('\n');
					}*/
					invalid_move = 1;
					break;
				}
			}
			if(!invalid_move)
				search(new_i, new_j, iter); // this piece can still be reached => search is ok
		}

		// search from here is done, put piece back
		board[new_i*n+new_j] = 0;
	}
}

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		fprintf(stderr, "usage: %s n\n", argv[0]);
		return -1;
	}

	n = atoi(argv[1]);
	squares = n*n;

	board = calloc(sizeof(int), squares);

	fputs(argv[0], stdout);
	putchar(' ');
	puts(argv[1]);
	puts("searching for solutions:");

	// only starting from 1 8th of the field, other solutions are flipped or rotated versions of these.
	int i;
	for(i = 0; i <= n/2; ++i)
	{
		int j;
		for(j = i; j <= n/2; ++j)
		{
			board[i*n + j] = 1;
			printf("search(%d, %d, 1);\n", i, j); // uncomment if n is few
			search(i, j, 1);
			board[i*n + j] = 0;
		}
	}

	printf("%d solutions found\n", solutions);
	free(board);

	return 0;
}

