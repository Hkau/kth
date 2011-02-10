#include "qsort.h"

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

pthread_mutex_t rand_lock = PTHREAD_MUTEX_INITIALIZER;

int prand()
{
	pthread_mutex_lock(&rand_lock);

	int r = rand();

	pthread_mutex_unlock(&rand_lock);

	return r;
}

void addsort(int *v, int first, int last);

void psort(int *v, int first, int last)
{
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

	--low_idx;
	v[first] = v[low_idx];
	v[low_idx] = pivot;

	addsort(v, first, low_idx-1);
	// don't need to find another worker for the second half
	psort(v, low_idx+1, last);
}

struct qsthread
{
	pthread_t thread;
	pthread_mutex_t wait_lock;
	pthread_cond_t start_signal;
	int *v;
	int first;
	int last;
} thread_default = {0, PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};

#define MAX_THREADS 16
struct qsthread thread_pool[MAX_THREADS];

size_t spawned_threads = 0;

struct qsthread *free_thread_pool[MAX_THREADS];
size_t num_free_threads = 0;

pthread_mutex_t order_lock = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t runthread_lock = PTHREAD_MUTEX_INITIALIZER;
bool runthread_arrived = false;
pthread_cond_t runthread_cond = PTHREAD_COND_INITIALIZER;
pthread_attr_t thread_attr;

void thread_barrier()
{
	pthread_mutex_lock(&runthread_lock);
	if(!runthread_arrived)
	{
		runthread_arrived = true;
		pthread_cond_wait(&runthread_cond, &runthread_lock);
	}
	else
	{
		pthread_cond_broadcast(&runthread_cond);
		runthread_arrived = false;
	}
	pthread_mutex_unlock(&runthread_lock);
}
void *run_thread(void *thread_ptr)
{
	struct qsthread *thread = (struct qsthread *)thread_ptr;

	pthread_mutex_lock(&thread->wait_lock);

	thread_barrier();

	while(1)
	{
		pthread_cond_wait(&thread->start_signal, &thread->wait_lock);
		//printf("    threaded: %p, %d, %d\n", thread->v, thread->first, thread->last);
		if(thread->v == NULL)
			pthread_exit(NULL);
		psort(thread->v, thread->first, thread->last);
		pthread_mutex_lock(&order_lock);
		if(num_free_threads == spawned_threads-1)
		{
			size_t i = 0;
			for(i = 0; i < num_free_threads; ++i)
			{
				free_thread_pool[i]->v = NULL;
				pthread_cond_signal(&free_thread_pool[i]->start_signal);
			}
			pthread_mutex_unlock(&order_lock);
			pthread_mutex_unlock(&thread->wait_lock);
			break;
		}
		free_thread_pool[num_free_threads++] = thread;
		pthread_mutex_unlock(&order_lock);
	}
	return NULL;
}

void addsort(int *v, int first, int last)
{
	pthread_mutex_lock(&order_lock);
	struct qsthread *thread = NULL;
	if(num_free_threads != 0)
	{
		thread = free_thread_pool[--num_free_threads];
	}
	else if(spawned_threads < MAX_THREADS)
	{
		thread_pool[spawned_threads] = thread_default;
//		thread_pool[spawned_threads].wait_lock = PTHREAD_MUTEX_INITIALIZER;
//		thread_pool[spawned_threads].start_signal = PTHREAD_COND_INITIALIZER;
		thread = &thread_pool[spawned_threads];
		pthread_create(&thread_pool[spawned_threads].thread, &thread_attr, run_thread, (void *)thread);

		thread_barrier();

		++spawned_threads;
	}

	pthread_mutex_unlock(&order_lock);

	if(thread != NULL)
	{
		thread->v = v;
		thread->first = first;
		thread->last = last;

		pthread_mutex_lock(&thread->wait_lock);
		pthread_cond_signal(&thread->start_signal);
		pthread_mutex_unlock(&thread->wait_lock);
	}
	else
		// no more threads available, work yourself.*/
		psort(v, first, last);
}

void quicksort(int *array, size_t n)
{
	pthread_attr_init(&thread_attr);
	pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);
	// init attr
	psort(array, 0, n-1);
	// quit thread pool, removeallthreads.
	size_t i;
	for(i = 0; i < spawned_threads; ++i)
	{
		pthread_join(thread_pool[i].thread, NULL);
		//pthread_mutex_destroy(&thread_pool[i].wait_lock);
		//pthread_cond_destroy(&thread_pool[i].start_signal);
	}
	pthread_attr_destroy(&thread_attr);
}

