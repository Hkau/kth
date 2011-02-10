#include "qsort.h"

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

pthread_mutex_t rand_lock = PTHREAD_MUTEX_INITIALIZER;

// synchronized random
int prand()
{
	pthread_mutex_lock(&rand_lock);

	int r = rand();

	pthread_mutex_unlock(&rand_lock);

	return r;
}

// find another thread to sort segment, or do self
void addsort(int *v, int first, int last);

// parallell qsort
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
	addsort(v, first, low_idx-1);
	// add second half of sort order
	addsort(v, first, low_idx-1);
}

// "thread" structure, each thread can be ordered to sort a list segment
// the locks are there because the thread can be able to wait for more
// work, the thread doesn't quit before all sorting is done (less overhead)
struct qsthread
{
	pthread_t thread;
	pthread_mutex_t wait_lock;
	pthread_cond_t start_signal;
	int *v;
	int first;
	int last;
} thread_default = {0, PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};

// thread pool
#define MAX_THREADS 16
struct qsthread thread_pool[MAX_THREADS];
size_t spawned_threads = 0;

// pointers to free threads
struct qsthread *free_thread_pool[MAX_THREADS];
size_t num_free_threads = 0;

// locks to prevent race conditions, mostly for addsort
pthread_mutex_t order_lock = PTHREAD_MUTEX_INITIALIZER;

// sync for starting threads, kind-of barrier stuff
pthread_mutex_t runthread_lock = PTHREAD_MUTEX_INITIALIZER;
bool runthread_arrived = false;
pthread_cond_t runthread_cond = PTHREAD_COND_INITIALIZER;
pthread_attr_t thread_attr;

// barrier function, only used to wait until threads are spawned,
// not used after
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

// thread starting point
void *run_thread(void *thread_ptr)
{
	struct qsthread *thread = (struct qsthread *)thread_ptr;

	pthread_mutex_lock(&thread->wait_lock);

	// let spawning thread know that we've started ok.
	thread_barrier();

	while(1)
	{
		pthread_cond_wait(&thread->start_signal, &thread->wait_lock);

		// NULL list means main thread wants us to quit
		if(thread->v == NULL)
			pthread_exit(NULL);
		// otherwise, sort thread
		psort(thread->v, thread->first, thread->last);

		pthread_mutex_lock(&order_lock);

		// if completing thread's current work means all started threads
		// are free, then work is done signal back to main thread
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
		// otherwise add thread back to pool and be available for more work
		free_thread_pool[num_free_threads++] = thread;
		pthread_mutex_unlock(&order_lock);
	}
	return NULL;
}

void addsort(int *v, int first, int last)
{
	// thread pool stuff
	pthread_mutex_lock(&order_lock);

	// find a thread that can take a beating (more work)
	struct qsthread *thread = NULL;

	// if one's free, that's our thread
	if(num_free_threads != 0)
	{
		thread = free_thread_pool[--num_free_threads];
	}
	// else, if we're able to spawn a new one, do it
	else if(spawned_threads < MAX_THREADS)
	{
		thread_pool[spawned_threads] = thread_default;
		thread = &thread_pool[spawned_threads];
		pthread_create(&thread_pool[spawned_threads].thread, &thread_attr, run_thread, (void *)thread);

		thread_barrier();

		++spawned_threads;
	}

	pthread_mutex_unlock(&order_lock);

	// if there's one to give work to, assign the order to it
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
		// no more threads available, work yourself.
		psort(v, first, last);
}

void quicksort(int *array, size_t n)
{
	// created threads should be joinable
	pthread_attr_init(&thread_attr);
	pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);

	// start sorting
	psort(array, 0, n-1);
	// quit thread pool, wait for all threads to finish.
	size_t i;
	for(i = 0; i < spawned_threads; ++i)
	{
		pthread_join(thread_pool[i].thread, NULL);
		//pthread_mutex_destroy(&thread_pool[i].wait_lock);
		//pthread_cond_destroy(&thread_pool[i].start_signal);
	}
	// remove allocated attr
	pthread_attr_destroy(&thread_attr);
}

