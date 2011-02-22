// Solution to The Near and Honeybees Problem
// Build: 'make honey'
// Running: ./honey [bees] [portions]
// Example: ./honey 40 10 -- 40 bees filling a pot with room for 10 "portions".

// Fairness: Semaphores aren't guaranteed to balance out workload, one process wakes up and does work. This could be the same few processes that gets to work all the time. If there's more work than workers, everyone should be able to get some work during each phase, so long as the process gets any time from the os.

#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#include <stdio.h>

sem_t filled;
sem_t empty;
sem_t bear_wait;
sem_t bee_mutex;

int portions = 7;

void millisleep(unsigned int ms)
{
	struct timespec req = {0}, rem = {0};
	req.tv_sec = (time_t)(ms / 1000);
	req.tv_nsec=((long)ms % 1000)*1000000L;

	nanosleep(&req, &rem);
}

void *bear(void *param)
{
	while(1)
	{
		sem_wait(&bear_wait); // wait until they're done

		printf("Bear - Time to eat!\n");
		millisleep(500);
		// eat all
		while(1)
		{
			int num;
			sem_getvalue(&filled, &num);
			if(num == 0)
				break;
			sem_wait(&filled);
			puts("Bear - Yum!");
			millisleep(100);
		}

		// give slots back
		int i;
		for(i = 0; i < portions; ++i)
			sem_post(&empty);
	}
}

void *bee(void *id)
{
	int num = (int)id;

	while(1)
	{
		sem_wait(&empty);

		millisleep(200 + (rand() % 500));

		sem_wait(&bee_mutex);

		int count;
		sem_getvalue(&filled, &count);
		sem_post(&filled);

		if(count == portions-1) // Did I fill the last portion?
		{
			sem_post(&bear_wait);
			printf("Bee #%d - Pot is full, bear, wake up!\n", num);
		}
		else
			printf("Bee #%d - After filling, there's still some more room!\n", num);

		sem_post(&bee_mutex);
	}
}

int main(int argc, char* argv[])
{
	int bees = 4;
	if(argc > 1)
		bees = atoi(argv[1]);
	if(argc > 2)
		portions = atoi(argv[2]);

	sem_init(&filled, 0, 0);
	sem_init(&empty, 0, portions);
	sem_init(&bee_mutex, 0, 1);
	sem_init(&bear_wait, 0, 0);

	printf("There are %d bees.\n", bees);

	pthread_t bee_threads[bees];

	int i;
	for(i = 0; i < bees; ++i)
	{
		pthread_create(&bee_threads[i], NULL, bee, (void *)(i+1));
	}

	pthread_t parent;
	pthread_create(&parent, NULL, bear, NULL);

	pthread_exit(NULL);
}

