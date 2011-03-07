#include "nbody.h"
#include "octree.h"

#include <stdlib.h>

float far = 10.f;

extern int render_octree;


int num_workers = 4;
#define MAX_WORKERS 16

void parse_args(int argc, char *argv[])
{
	if(argc > 1)
	{
		num_bodies = atoi(argv[1]);
		if(argc > 2)
		{
			num_steps = atoi(argv[2]);
			if(argc > 3)
			{
				far = atof(argv[3]);
				if(argc > 4)
				{
					num_workers = atoi(argv[4]);
					if(argc > 5)
					{
						allow_negative = (atoi(argv[5]) != 0);
						if(argc > 6)
						{
							render_octree = (atoi(argv[6]) != 0);
						}
					}
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
	oct_init(bounds);
	int i;
	for(i = 0; i < num_bodies; ++i)
		oct_add(top, &body[i]);

	barrier_waits = num_workers;
	for(i = 0; i < num_workers-1; ++i) // in addition to main thread, start num_workers-1 threads
		pthread_create(&threads[i], NULL, &thread, (void*)i+1);

	master_barrier();
}

void client_exit()
{
	oct_quit();
}

void calc_body_forces(body_t *body, octree *node)
{
	if(node->count == 0)
		return;
	vec3f close;

	if(node->max.x < body->pos.x)
		close.x = node->max.x;
	else if(node->min.x > body->pos.x)
		close.x = node->min.x;
	else
		close.x = body->pos.x;

	if(node->max.y < body->pos.y)
		close.y = node->max.y;
	else if(node->min.y > body->pos.y)
		close.y = node->min.y;
	else
		close.y = body->pos.y;

	if(node->max.z < body->pos.z)
		close.z = node->max.z;
	else if(node->min.z > body->pos.z)
		close.z = node->min.z;
	else
		close.z = body->pos.z;

	float dist_sq = vec3f_len_sq(close);

	if(dist_sq >= sq(far))
	{
		vec3f center = vec3f_scale(node->center, 1./node->count);
		float force = G * body->mass * (node->mass/vec3f_len_sq(vec3f_sub(center, body->pos)));
		
		vec3f delta = vec3f_scale(vec3f_sub(center, body->pos), force);
		body->force = vec3f_add(body->force, delta);
		return;
	}

	// add all non-blocks in range
	if(node->children == NULL)
	{
		body_t *b;
		for(b = node->bodies; b != NULL; b = b->next)
		{
			float force = G * body->mass * b->mass / (max(0.001f, vec3f_len_sq(vec3f_sub(b->pos, body->pos))));

			// units closer than sqrt(5) repel eachother to spread out, ish
			if(vec3f_len_sq(vec3f_sub(b->pos, body->pos)) < 5.f)
				force = -100*force;

			vec3f delta = vec3f_scale(vec3f_sub(b->pos, body->pos), force);
			body->force = vec3f_add(body->force, delta);
		}
		return;
	}
	int i;
	for(i = 0; i < 8; ++i)
		calc_body_forces(body, &node->children[i]);
}

void thread_calc_forces()
{
	while(true)
	{
		pthread_mutex_lock(&iter_lock);
		int i = global_iter;
		int limit = min(i+10, num_bodies);
		global_iter += 10;
		pthread_mutex_unlock(&iter_lock);

		for(; i < limit; ++i)
		{
			body[i].force = vec3f_zero();
			calc_body_forces(&body[i], top);
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
	oct_move(body, pos);
	//body->pos = pos;
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

		for(; i < limit; ++i)
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

