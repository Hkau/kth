#include "nbody.h"

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int num_workers = 4;
#define MAX_WORKERS 16

void parse_args(int argc, char *argv[])
{
	// ./nbody2 <num_bodies> <num_steps> <num_workers> <allow_negative_mass>
	if(argc > 1)
	{
		num_bodies = atoi(argv[1]);
		if(argc > 2)
		{
			num_steps = atoi(argv[2]);
			if(argc > 3)
			{
				num_workers = atoi(argv[3]);
				if(num_workers > MAX_WORKERS)
				{
					fprintf(stderr, "warning: num_workers more than max, %d, setting to max.\n", MAX_WORKERS);
					num_workers = MAX_WORKERS;
				}
				if(argc > 4)
				{
					allow_negative = (atoi(argv[4]) != 0);
				}
			}
		}
	}
}

enum mode
{
	CALC_FORCES,
	ADD_VELOCITY,
	EXIT // unused because program exits, but could be used to close workers
} thread_oper;

int global_iter;
pthread_cond_t master_signal, thread_signal;
pthread_mutex_t iter_lock = PTHREAD_MUTEX_INITIALIZER;

int barrier_waits;
pthread_mutex_t barrier_lock = PTHREAD_MUTEX_INITIALIZER;

void thread_barrier()
{
	pthread_mutex_lock(&barrier_lock);
	if(--barrier_waits == 0)
	{
		pthread_cond_signal(&master_signal);
	}
	pthread_cond_wait(&thread_signal, &barrier_lock);

	pthread_mutex_unlock(&barrier_lock);
}

void master_barrier()
{
	pthread_mutex_lock(&barrier_lock);
	if(--barrier_waits != 0)
	{
		pthread_cond_wait(&master_signal, &barrier_lock);
	}
	pthread_mutex_unlock(&barrier_lock);
}

void master_wake()
{
	barrier_waits = num_workers;
	pthread_cond_broadcast(&thread_signal);
}

void thread_calc_forces();
void thread_add_velocity();

void *thread(void *args)
{
	//int threadid = (int) args;
	while(true)
	{
		thread_barrier(); // Wait for work;

		switch(thread_oper)
		{
			case CALC_FORCES:
				//printf("thread %d calculating forces.\n", threadid);
				thread_calc_forces();
				break;
			case ADD_VELOCITY:
				//printf("thread %d adding velocity.\n", threadid);
				thread_add_velocity();
				break;
			default:
				break;
		}

		if(thread_oper == EXIT)
			break;
	}
	//printf("thread %d done.\n", threadid);

	return NULL;
}

pthread_t threads[MAX_WORKERS-1];

void client_start()
{
	int i;
	barrier_waits = num_workers;
	for(i = 0; i < num_workers-1; ++i) // in addition to main thread, start num_workers-1 threads
		pthread_create(&threads[i], NULL, &thread, (void*)i+1);
	master_barrier();
}

void client_exit()
{
	// kill threads
	//thread_oper = EXIT;
	//master_wake();

	// nvm, threads exit automatically when main does, and there's no need to wait for them since every thread is sleeping here
}

void thread_calc_forces()
{
	while(true)
	{
		pthread_mutex_lock(&iter_lock);
		int i = global_iter;
		int limit = min(i + 10, num_bodies);
		global_iter += 10;
		pthread_mutex_unlock(&iter_lock);

		for(; i < limit; ++i)
		{
			body[i].force = vec3f_zero();
			int j;
			for(j = 0; j < num_bodies; ++j)
			{
				if(i == j)
					continue;

				float force = G * body[i].mass * body[j].mass / 
					(max(0.001f, (sq(body[i].pos.x - body[j].pos.x) + sq(body[i].pos.y - body[j].pos.y) + sq(body[i].pos.z - body[j].pos.z))));

				//units closer than 5 from eachother repel and kind-of spread out
				if(sq(body[i].pos.x - body[j].pos.x) + sq(body[i].pos.y - body[j].pos.y) + sq(body[i].pos.z - body[j].pos.z) < 5.f)
					force = -100*force;

				//vec3f delta = vec3f_scale(vec3f_sub(body[j].pos, body[i].pos), -force);
				vec3f delta = vec3f_scale(vec3f_sub(body[j].pos, body[i].pos), force);
				body[i].force = vec3f_add(body[i].force, delta);
			}
		}

		if(i >= num_bodies)
			break;
	}
}

void calc_forces()
{
	global_iter = 0;
	thread_oper = CALC_FORCES;

	//puts("main thread: calc forces");

	master_wake();
	// wake up other threads
	thread_calc_forces(); // help out as well

	// wait for other threads to finish and put them to sleep
	master_barrier();
}

void body_move_pos(body_t *body, vec3f pos)
{
	body->pos = pos;
}

void thread_add_velocity()
{
	while(true)
	{
		pthread_mutex_lock(&iter_lock);
		int i = global_iter;
		int limit = min(i+10, num_bodies);
		global_iter += 10;
		pthread_mutex_unlock(&iter_lock);

		for(i = i; i < limit; ++i)
		{
			vec3f accel = vec3f_scale(body[i].force, 1.f/abs(body[i].mass)); // a = f/m
			body[i].vel = vec3f_add(body[i].vel, vec3f_scale(accel, 0.5f));
			body_move(&body[i], vec3f_add(body[i].pos, body[i].vel));
			body[i].vel = vec3f_add(body[i].vel, vec3f_scale(accel, 0.5f));
		}

		if(i >= num_bodies)
			break;
	}
}

void add_velocity()
{
	global_iter = 0;
	thread_oper = ADD_VELOCITY;

	//puts("main thread: add velocity");

	master_wake();
	thread_add_velocity();

	// wait for others to finish and make them wait for additional work
	master_barrier();
}
