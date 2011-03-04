#include "gui.h"

#include "nbody.h"
#include <stdlib.h>
#include <stdio.h>

int numSteps = -1;

body_t *body = NULL;
size_t num_bodies = 2000;
float bounds = 10000.f;

// Gravitational constant
float G = 0.0000000000001;

float turbo = 1.f;
bool allow_negative = false;

int main()
{
	gui_init("nbody1");

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
		body[i].pos.x = rand() % 20 - 10;
		body[i].pos.y = rand() % 20 - 10;
		body[i].pos.z = rand() % 20 - 10;
	}

	// SLASH
	/*
	for(i = 0; i < num_bodies/2; ++i)
	{
		body[i].mass = rand();
		body[i].pos.x = -50+ rand() % 20;
		body[i].pos.y = -50 + rand() % 20;
		body[i].pos.z = -50 + rand() % 20;
	}
	for(i = num_bodies/2; i < num_bodies; ++i)
	{
		body[i].mass = -rand();
		body[i].pos.x = 50 + rand() % 20;
		body[i].pos.y = 50 + rand() % 20;
		body[i].pos.z = 50 + rand() % 20;
	}*/

	while(gui_update())
	{
		// random mass flip
		if(allow_negative)
			body[rand() % num_bodies].mass *= -1.f;
		// simulate stuff
		for(i = 0; i < num_bodies; ++i)
		{
			body[i].force = vec3f_zero();
			int j;
			for(j = 0; j < num_bodies; ++j)
			{
				if(i == j)
					continue;

				float force = G * body[i].mass * body[j].mass / 
					(0.001f + sq(body[i].pos.x - body[j].pos.x) + sq(body[i].pos.y - body[j].pos.y) + sq(body[i].pos.z - body[j].pos.z));

				//vec3f delta = vec3f_scale(vec3f_sub(body[j].pos, body[i].pos), -force);
				vec3f delta = vec3f_scale(vec3f_sub(body[j].pos, body[i].pos), force);
				body[i].force = vec3f_add(body[i].force, delta);
			}
		}

		for(i = 0; i < num_bodies; ++i)
		{
			body[i].vel = vec3f_add(body[i].vel, vec3f_scale(body[i].force, 0.5f/abs(body[i].mass)));
			body[i].pos = vec3f_add(body[i].pos, vec3f_scale(body[i].vel, turbo));
			body[i].vel = vec3f_add(body[i].vel, vec3f_scale(body[i].force, 0.5f/abs(body[i].mass)));

			float bounce = 1/2.f;//10.f;
			if(body[i].pos.x < -bounds)
			{
				body[i].pos.x = -bounds;
				body[i].vel.x = -body[i].vel.x * bounce;
			}
			else if(body[i].pos.x > bounds)
			{
				body[i].pos.x = bounds;
				body[i].vel.x = -body[i].vel.x * bounce;
			}

			if(body[i].pos.y < -bounds)
			{
				body[i].pos.y = -bounds;
				body[i].vel.y = -body[i].vel.y * bounce;
			}
			else if(body[i].pos.y > bounds)
			{
				body[i].pos.y = bounds;
				body[i].vel.y = -body[i].vel.y * bounce;
			}

			if(body[i].pos.z < -bounds)
			{
				body[i].pos.z = -bounds;
				body[i].vel.z = -body[i].vel.z * bounce;
			}
			else if(body[i].pos.z > bounds)
			{
				body[i].pos.z = bounds;
				body[i].vel.z = -body[i].vel.z * bounce;
			}
		}

		// if there's a limit, count down and break if reached
		if(numSteps > 0 && (numSteps-- == 0))
			break;
	}

	gui_quit();

	free(body);

	return 0;
}

