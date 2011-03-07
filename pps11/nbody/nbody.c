#include "nbody.h"
#include "gui.h"

#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <sys/times.h>

// common nbody functions

int num_steps = -1;

body_t *body = NULL;
size_t num_bodies = 500;
bool allow_negative = false;
float bounds = 100.f;

// Gravitational constant
float G = 0.000000000001;

void nbody_init(int count)
{
	num_bodies = count;
	if(num_bodies < 0)
		num_bodies = 0;

	if(num_bodies > 0)
		body = calloc(num_bodies, sizeof(body_t)); // init bodies to 0

	int i;
	for(i = 0; i < num_bodies; ++i)
	{
		// Random mass and random starting positions. :)
		// (Where random isn't seeded.)
		if(allow_negative)
		{
			body[i].mass = 2*(rand()-RAND_MAX/2);
			if(body[i].mass == 0)
				body[i].mass = 1;
		}
		else
			body[i].mass = rand() + 1;

		body[i].pos.x = (rand() % (2*(int)bounds)) - (int)bounds;
		body[i].pos.y = (rand() % (2*(int)bounds)) - (int)bounds;
		body[i].pos.z = (rand() % (2*(int)bounds)) - (int)bounds;
	}
}

void nbody_quit()
{
	free(body);
}

void body_move(body_t *body, vec3f pos)
{
		// wall bounce when hitting bounds
		float bounce = 1/2.f;//10.f;

		if(pos.x < -bounds)
		{
			pos.x = -bounds;
			body->vel.x = -body->vel.x * bounce;
		}
		else if(pos.x > bounds)
		{
			pos.x = bounds;
			body->vel.x = -body->vel.x * bounce;
		}

		if(pos.y < -bounds)
		{
			pos.y = -bounds;
			body->vel.y = -body->vel.y * bounce;
		}
		else if(pos.y > bounds)
		{
			pos.y = bounds;
			body->vel.y = -body->vel.y * bounce;
		}

		if(pos.z < -bounds)
		{
			pos.z = -bounds;
			body->vel.z = -body->vel.z * bounce;
		}
		else if(pos.z > bounds)
		{
			pos.z = bounds;
			body->vel.z = -body->vel.z * bounce;
		}

		body_move_pos(body, pos);
}

int main(int argc, char* argv[])
{
	parse_args(argc, argv); // Done differently depending on application

	nbody_init(num_bodies);

	gui_init("nbody1");

/*	// Make some clumped bodies with initial velocity for a more interesting simulation
	for(i = 0; i < num_bodies/4; ++i)
	{
		body[i].pos.x = -50 + rand() % 20;
		body[i].pos.y = -50 + rand() % 20;
		body[i].pos.z = -50 + rand() % 20;
		body[i].vel.x = 0.05;
	}
	for(i = num_bodies/4; i < num_bodies/2; ++i)
	{
		body[i].pos.x = 30 + rand() % 20;
		body[i].pos.y = 30 + rand() % 20;
		body[i].pos.z = 30 + rand() % 20;
		body[i].vel.x = -0.05;
	}*/

	float delta = 0.f;

	clock_t start = times(NULL);

	client_start();

	int iterations = 0;
	while(gui_update())
	{
		// simulate stuff
		delta += turbo;
		while(delta >= 1.f)
		{
			// random mass flip
			//if(allow_negative)
			//		body[rand() % num_bodies].mass *= -1.f;

			calc_forces();

			add_velocity();

			delta -= 1.f;
			++iterations;
		}

		// if there's a limit, count down and break if reached
		if(num_steps > 0 && (--num_steps == 0))
			break;
	}

	clock_t stop = times(NULL);

	fputs(argv[0], stdout);
	int i;
	for(i = 1; i < argc; ++i)
	{
		fputc(' ', stdout);
		fputs(argv[i], stdout);
	}

	long ticks_per_sec = sysconf(_SC_CLK_TCK);
	printf("\n%d iterations.\n", iterations);
	clock_t time = stop - start;
	printf("elapsed: %f seconds.\navg: %f seconds per iteration.\n", ((float)(time))/ticks_per_sec, ((float)(time))/ticks_per_sec/iterations);

	client_exit();
	gui_quit();

	free(body);

	return 0;
}

