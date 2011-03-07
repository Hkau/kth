#include "nbody.h"
#include "octree.h"

#include <stdlib.h>

float far = 10.f;

extern int render_octree;

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
					allow_negative = (atoi(argv[4]) != 0);
					if(argc > 5)
					{
						render_octree = (atoi(argv[5]) != 0);
					}
				}
			}
		}
	}
}

void client_start()
{
	oct_init(bounds);
	int i;
	for(i = 0; i < num_bodies; ++i)
		oct_add(top, &body[i]);
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

void calc_forces()
{
	int i;

	for(i = 0; i < num_bodies; ++i)
	{
		body[i].force = vec3f_zero();
		calc_body_forces(&body[i], top);
	}
	// barrier();
}

void body_move_pos(body_t *body, vec3f pos)
{
	oct_move(body, pos);
	//body->pos = pos;
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

