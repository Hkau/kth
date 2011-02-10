#include <stdio.h>
#include <pthread.h>

pthread_t std_thread, file_thread;

#define BUFFER_SIZE 1024
char buffer[BUFFER_SIZE];
int eof = 0;

// barrier
pthread_mutex_t block = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t write_signal = PTHREAD_COND_INITIALIZER;
pthread_cond_t read_signal = PTHREAD_COND_INITIALIZER;
int count;
int num_files = 1;

void wait_read()
{
	pthread_mutex_lock(&block);

	if(--count == 0)
		pthread_cond_signal(&write_signal);

	pthread_cond_wait(&read_signal, &block);
	pthread_mutex_unlock(&block);
}
void wait_write()
{
	pthread_mutex_lock(&block);

	if(count != 0)
		pthread_cond_wait(&write_signal, &block);

	if(fgets(buffer, BUFFER_SIZE, stdin) == NULL)
		eof = 1;

	pthread_cond_broadcast(&read_signal);

	count = num_files;

	pthread_mutex_unlock(&block);
}

void* copy(void *desc)
{
	FILE *file = (FILE *) desc;

	while(1)
	{
		wait_read();

		if(eof)
			break;

		fputs(buffer, file);
	}

	return NULL;
}

int main(int argc, char* argv[])
{
	FILE *file = NULL;

	if(argc > 1)
	{
		if(argc != 2)
		{
			fprintf(stderr, "usage: tee 'file'\n");
			return 1;
		}
		file = fopen(argv[1], "w");
		if(file == NULL)
		{
			perror("can't open file");
			return 1;
		}
		++ num_files;
	}

	count = num_files;

	pthread_create(&std_thread, NULL, copy, stdout);

	if(file != NULL)
		pthread_create(&file_thread, NULL, copy, file);

	while(!eof)
	{
		wait_write();
	}

	pthread_join(std_thread, NULL);
	if(file != NULL)
		pthread_join(file_thread, NULL);

	return 0;
}

