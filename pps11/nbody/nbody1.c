#include "nbody.h"

#include <stdlib.h>

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
				allow_negative = (atoi(argv[3]) != 0);
			}
		}
	}
}

void client_start()
{
}
void client_exit()
{
}

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
				(max(0.001f, (sq(body[i].pos.x - body[j].pos.x) + sq(body[i].pos.y - body[j].pos.y) + sq(body[i].pos.z - body[j].pos.z))));

			//units closer than sqrt(5) from eachother repel and kind-of spread out
			if(sq(body[i].pos.x - body[j].pos.x) + sq(body[i].pos.y - body[j].pos.y) + sq(body[i].pos.z - body[j].pos.z) < 5.f)
				force = -100*force;

			//vec3f delta = vec3f_scale(vec3f_sub(body[j].pos, body[i].pos), -force);
			vec3f delta = vec3f_scale(vec3f_sub(body[j].pos, body[i].pos), force);
			body[i].force = vec3f_add(body[i].force, delta);
		}
	}
	// barrier();
}

void body_move_pos(body_t *body, vec3f pos)
{
		body->pos = pos;
}

void add_velocity()
{
	int i;
	for(i = 0; i < num_bodies; ++i)
	{
		vec3f accel = vec3f_scale(body[i].force, 1.f/abs(body[i].mass)); // a = f/m
		body[i].vel = vec3f_add(body[i].vel, vec3f_scale(accel, 0.5f));
		body_move(&body[i], vec3f_add(body[i].pos, body[i].vel));
		body[i].vel = vec3f_add(body[i].vel, vec3f_scale(accel, 0.5f));
	}
	// barrier();
}

