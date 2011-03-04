#include "gui.h"

#include "nbody.h"
#include <stdlib.h>
#include <stdio.h>

#include <sys/times.h>

float bounds = 1000.f;

// Gravitational constant
float G = 0.000000000001;

void calc_forces()
{
	int i;

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
	// barrier();
}

void add_velocity()
{
	int i;
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
	// barrier();
}

int main()
{
	// parse args

	nbody_init(num_bodies, true);

	gui_init("nbody1");

	// Make some clumped bodies with initial velocity for a more interesting simulation
	int i;
	for(i = 0; i < num_bodies/4; ++i)
	{
		body[i].pos.x = -50 + rand() % 20;
		body[i].pos.y = -50 + rand() % 20;
		body[i].pos.z = -50 + rand() % 20;
		body[i].vel.x = 0.5;
	}
	for(i = num_bodies/4; i < num_bodies/2; ++i)
	{
		body[i].pos.x = 30 + rand() % 20;
		body[i].pos.y = 30 + rand() % 20;
		body[i].pos.z = 30 + rand() % 20;
		body[i].vel.x = -0.5;
	}
	/*for(i = num_bodies/2; i < num_bodies; ++i)
	{
		body[i].mass = -rand();
		body[i].pos.x = 50 + rand() % 20;
		body[i].pos.y = 50 + rand() % 20;
		body[i].pos.z = 50 + rand() % 20;
	}*/

	while(gui_update())
	{
		// random mass flip
		//if(allow_negative)
		//		body[rand() % num_bodies].mass *= -1.f;

		// simulate stuff
		calc_forces();

		add_velocity();

		// if there's a limit, count down and break if reached
		if(numSteps > 0 && (numSteps-- == 0))
			break;
	}

	gui_quit();

	free(body);

	return 0;
}

