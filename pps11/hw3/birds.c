// Solution to The Hungry Birds problem.
// Build: 'make birds'
// Running: ./birds [baby birds] [initial worms in the bowl]
// Example: ./birds 100 2   -- 100 birds sharing a bowl with 2 worms initially.

// Note: The parent bird fills the bowl with a random number of worms to the bowl each time, instead of a constant amount. (between 1 and 2*[initial worms in bowl]). This was thought to be a good exercise and ensure that the solution was more general.
// Fairness: The solution isn't guaranteed to be fair, as semaphores don't say that the thread (baby bird) who's been waiting the longest, is the one that gets to go when the semaphore gets signaled. One bird could theoretically be locked out. But more probably, some birds will get to wait less than others.

#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#include <stdio.h>

int birds = 4;
int num_worms = 7;

sem_t worms;
sem_t wake;
sem_t mutex_take;

void millisleep(unsigned int ms)
{
	struct timespec req = {0}, rem = {0};
	req.tv_sec = (time_t)(ms / 1000);
	req.tv_nsec=((long)ms % 1000)*1000000L;

	nanosleep(&req, &rem);
}

void *parent_bird(void *param)
{
	while(1)
	{
		sem_wait(&wake);
		puts("Tweet! My babies need food!");
		millisleep(4000);
		int found = 1 + rand() % (2 * num_worms);
		printf("Tweet! I found %d worms!\n", found);
		millisleep(1000);
		while(found --)
		{
			sem_post(&worms);
		}
		millisleep(100);
		puts("Tweet! Now I'm so tired. Zzzz...");
		millisleep(1000);
	}
	
}

void *bird(void *id)
{
	int num = (int)id;

	while(1)
	{
		printf("Bird #%d plays around for a bit.\n", num);
		millisleep(500 + (rand() % 2000));

		printf("Bird #%d - I'm starving, I should go and get some food!\n", num);
		millisleep(200 + (rand() % 1000));

		// time to pick a worm, only one bird may examine the bowl at the same time
		sem_wait(&mutex_take);
		int count;
		sem_getvalue(&worms, &count);
//		printf("Bird #%d - There are %d worms.\n", num, count);
		if(count == 0)
		{
			// no worms, signal to parent -- only one bird does this before the bowl is refilled
			printf("Bird #%d - The bowl is empty. Hold on guys! -- CHIIIIIRP!!\n", num);
			sem_post(&wake);
		}
		else
			// worms available
			printf("Bird #%d - Sweet! Looks like there are some worms here!\n", num);
		// take a worm
		sem_wait(&worms);
		// let others do so as well
		sem_post(&mutex_take);
		printf("Bird #%d - Mmmm, that's a tasty worm!\n", num);
		millisleep(200);
	}
}

int main(int argc, char* argv[])
{
	if(argc > 1)
		birds = atoi(argv[1]);
	if(argc > 2)
		num_worms = atoi(argv[2]);

	sem_init(&worms, 0, num_worms);
	sem_init(&wake, 0, 0);
	sem_init(&mutex_take, 0, 1);

	printf("There are %d birds.\n", birds);

	pthread_t babies[birds];

	int i;
	for(i = 0; i < birds; ++i)
	{
		pthread_create(&babies[i], NULL, bird, (void *)(i+1));
	}

	pthread_t parent;
	pthread_create(&parent, NULL, parent_bird, NULL);

	pthread_exit(NULL);
}

