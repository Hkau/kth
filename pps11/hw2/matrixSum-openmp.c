/* matrix summation using pthreads

features: uses a barrier; the Worker[0] computes
the total sum from partial sums computed by Workers
and prints the total sum to the standard output

usage on Solaris:
gcc matrixSum.c -lpthread -lposix4
a.out size numWorkers

 */

#include <omp.h>

double start_time, end_time;

#include <stdio.h>
#ifndef MAXSIZE
#define MAXSIZE 10000
#endif
#define MAXWORKERS 8   /* maximum number of workers */

int numWorkers;
int size; 
int matrix[MAXSIZE][MAXSIZE];
void *Worker(void *); // unused

/* read command line, initialize, and create threads */
int main(int argc, char *argv[]) {
	int i, j, total = 0;
	int total_low_i = 0, total_low_j = 0, total_high_i = 0, total_high_j = 0;

	/* read command line args if any */
	size = (argc > 1)? atoi(argv[1]) : MAXSIZE;
	numWorkers = (argc > 2)? atoi(argv[2]) : MAXWORKERS;
	if (size > MAXSIZE) size = MAXSIZE;
	if (numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;

// disabled for testing with OMP_NUM_THREADS, also because the host environment is better at deciding this limit for us.
//	omp_set_num_threads(numWorkers);

	/* initialize the matrix */
	for (i = 0; i < size; i++) {
		//  printf("[ ");
		for (j = 0; j < size; j++) {
			matrix[i][j] = rand() % 99;
			//	  printf(" %d", matrix[i][j]);
		}
		//	  printf(" ]\n");
	}

	start_time = omp_get_wtime();
	#pragma omp parallel shared(i)
	{
		int low_i = 0, low_j = 0, high_i = 0, high_j = 0;
		int thread_id = omp_get_thread_num();

		#pragma omp for reduction(+:total) private(j) schedule(static)
		for (i = 0; i < size; i++)
		{
//			printf("%d: %d\n", thread_id, i);
			for (j = 0; j < size; j++){
				total += matrix[i][j];
//				printf("%d: matrix[%d][%d] = %d\n", thread_id, i, j, matrix[i][j]);
				if(matrix[i][j] < matrix[low_i][low_j])
				{
					low_i = i;
					low_j = j;
				}
				else if(matrix[i][j] > matrix[high_i][high_j])
				{
					high_i = i;
					high_j = j;
				}
			}
		}

		#pragma omp critical
		{
			//printf("%d: thread min, %d, is located at matrix[%d][%d]\n", thread_id, matrix[low_i][low_j], low_i, low_j);
			//printf("%d: thread max, %d, is located at matrix[%d][%d]\n", thread_id, matrix[high_i][high_j], high_i, high_j);
			if(matrix[low_i][low_j] < matrix[total_low_i][total_low_j])
			{
				total_low_i = low_i;
				total_low_j = low_j;
			}

			if(matrix[high_i][high_j] > matrix[total_high_i][total_high_j])
			{
				total_high_i = high_i;
				total_high_j = high_j;
			}
		}
	}
	end_time = omp_get_wtime();

	printf("the total is %d\n", total);
	printf("the min, %d, is located at matrix[%d][%d]\n", matrix[total_low_i][total_low_j], total_low_i, total_low_j);
	printf("the max, %d, is located at matrix[%d][%d]\n", matrix[total_high_i][total_high_j], total_high_i, total_high_j);
	printf("it took %g seconds\n", end_time - start_time);

//	printf("%g\n", end_time - start_time);
}

