#include <stdio.h>
#include <stdlib.h>

#include <pthread.h> // yes, the problem is that hard.

int n;
int squares;
struct
{
	int i, j;
} moves[] = {{0, -3}, {0, 3}, {-3, 0}, {3, 0},
	{-2, -2}, {-2, 2}, {2, -2}, {2, 2}};
// These moves are |3| horizontally and vertically and |2| diagonally.

int num_moves = sizeof(moves)/sizeof(moves[0]);

pthread_mutex_t sol_lock;
int solutions = 0;

int max_depth = 0;

unsigned long total_iter = 0;

int reachable(int *board, int i, int j)
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
void search(int *board, int i, int j, int iter)
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
			pthread_mutex_lock(&sol_lock);
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
			pthread_mutex_unlock(&sol_lock);
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

				if(!reachable(board, check_i, check_j))
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
				search(board, new_i, new_j, iter); // this piece can still be reached => search is ok
		}

		// search from here is done, put piece back
		board[new_i*n+new_j] = 0;
	}
}

int num_threads = 1;
pthread_mutex_t job_lock = PTHREAD_MUTEX_INITIALIZER;
int iter_i = 0, iter_j = -1; // will increase to 0,0 in first iteration

pthread_mutex_t barrier_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t barrier_signal = PTHREAD_COND_INITIALIZER;
int barrier_count = 0;

void barrier()
{
	pthread_mutex_lock(&barrier_lock);

	barrier_count++;

	if(barrier_count == num_threads)
	{
		pthread_cond_broadcast(&barrier_signal);
		barrier_count = 0; // reset barrier
	}
	else
		pthread_cond_wait(&barrier_signal, &barrier_lock);

	pthread_mutex_unlock(&barrier_lock);
}

void *run(void *args)
{
	int *board = calloc(sizeof(int), squares);

	while(1)
	{
		pthread_mutex_lock(&job_lock);
		// only starting from 1 8th of the field, other solutions are flipped or rotated versions of these.
		iter_j++;
		if(iter_j > n/2)
		{
			iter_i ++;
			if(iter_i > n/2)
			{
				pthread_mutex_unlock(&job_lock);
				break;
			}
			iter_j = iter_i;
		}
		pthread_mutex_unlock(&job_lock);

		board[iter_i*n + iter_j] = 1;
		printf("search(%d, %d, 1);\n", iter_i, iter_j); // uncomment if n is few
		search(board, iter_i, iter_j, 1);
		board[iter_i*n + iter_j] = 0;
	}

	free(board);

	barrier();

	return 0;
}

int main(int argc, char *argv[])
{
	if(argc != 2 && argc != 3)
	{
		fprintf(stderr, "usage: %s n [num_threads]\n", argv[0]);
		return -1;
	}

	n = atoi(argv[1]);
	if(argc == 3)
		num_threads = atoi(argv[2]);


	squares = n*n;

	fputs(argv[0], stdout);
	putchar(' ');
	puts(argv[1]);
	puts("searching for solutions:");

	pthread_t threads[num_threads];

	int i;
	for(i = 0; i < num_threads-1; ++i)
	{
		pthread_create(&threads[i], NULL, &run, NULL);
	}
	run(NULL);


	printf("%d solutions found\n", solutions);

	return 0;
}

