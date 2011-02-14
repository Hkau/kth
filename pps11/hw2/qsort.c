#include "qsort.h"

#include <omp.h>
#include <stdbool.h>
#include <stdlib.h>

// synchronized random
int prand()
{
	int r;

	#pragma omp critical
	r = rand();

	return r;
}

#include <stdio.h>
// parallell qsort
void ompsort(int *v, int first, int last)
{
	//printf("%d: (%p, %d, %d)\n", omp_get_thread_num(), v, first, last);
	if (first >= last)
		return;

	// insertion sort goes here, cause it's faster.
	if(last - first < 20)
	{
		size_t i;
		for(i = first+1; i <= last; ++i)
		{
			size_t j;
			for(j = i; j > first; --j)
			{
				if(v[j-1] < v[j])
					break;
				int tmp = v[j-1];
				v[j-1] = v[j];
				v[j] = tmp;
			}
		}
		return;
	}

	// the n^2 threat is way more scary for large lists so
	// just pick whatever pivot when lists are fairly small.
	size_t pivot_idx = first + ((last - first > 64) ? prand() % (last-first+1) : 0);

	// partition around pivot_idx
	// swap pivot to first place, so it can be moved back to middle
	int pivot = v[pivot_idx];
	v[pivot_idx] = v[first];
	v[first] = pivot;

	size_t low_idx = first + 1;
	size_t high_idx = last;

	while(low_idx <= high_idx)
	{
		if(v[low_idx] >= pivot)
		{
			int tmp = v[high_idx];
			v[high_idx] = v[low_idx];
			v[low_idx] = tmp;

			-- high_idx;
		}
		else
			++ low_idx;
	}

	// swap pivot back to middle
	--low_idx;
	v[first] = v[low_idx];
	v[low_idx] = pivot;

	// sort elements before it (first "half")
	#pragma omp task
	ompsort(v, first, low_idx-1);
	// add second half of sort order
	#pragma omp task
	ompsort(v, low_idx+1, last);
	//#pragma omp taskwait
}

void quicksort(int *array, size_t n)
{
	#pragma omp parallel
	{
		#pragma omp single
		ompsort(array, 0, n-1);
	}
	// implicit barrier
}

