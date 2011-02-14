#include "qsort.h"
#include <stdio.h>
#include <stdlib.h>

#include <omp.h>

// warning: this might be too large in some cases, in that case
// try with a smaller number. This (128*1024*1024) took around 
// 55 seconds before multithreading, and ~30 seconds after.

#ifndef ARRAY_SIZE
#define ARRAY_SIZE (128*1024*1024)
#endif

int main()
{
	int size = ARRAY_SIZE;
	int *stuff = malloc(size * sizeof(int));
	if(stuff == NULL)
	{
		perror("uh?");
		return 1;
	}
	size_t i;
	for(i = 0; i < size; ++i)
	{
		stuff[i] = i;
	}
	// shuffle
	for(i = size-1; i > 0; --i)
	{
		size_t idx = rand() % i;

		int tmp = stuff[idx];
		stuff[idx] = stuff[i];
		stuff[i] = tmp;
	}

	double start = omp_get_wtime();
	quicksort(stuff, size);
	double stop = omp_get_wtime();
	printf("%g\n", (stop - start));

	for(i = 0; i < size; ++i)
	{
		if(stuff[i] != i)
		{
			fprintf(stderr, "broken, stuff[%d] = %d, should be %d\n.", i, stuff[i], i);
			return 1;
		}
	}
	free(stuff);
	return 0;
}

