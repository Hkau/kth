#include "qsort.h"
#include <stdio.h>
#include <stdlib.h>

// warning: this might be too large in some cases, in that case
// try with a smaller number. This (32*1024*1024) took around 
// 18 seconds on my laptop before multithreading.
// After it...
#define size (1024*1024*32)

int main()
{
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

	quicksort(stuff, size);

	for(i = 0; i < size; ++i)
	{
		if(stuff[i] != i)
		{
			fprintf(stderr, "broken, stuff[%d] = %d, should be %d\n.", i, stuff[i], i);
			return 1;
		}
	}
	return 0;
}

