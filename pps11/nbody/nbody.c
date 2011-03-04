#include "nbody.h"

// common nbody functions

int numSteps = -1;

#include <stdlib.h>

body_t *body = NULL;
size_t num_bodies = 2000;

void nbody_init(int count, bool allow_negative)
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
		body[i].pos.x = rand() % 200 - 100;
		body[i].pos.y = rand() % 200 - 100;
		body[i].pos.z = rand() % 200 - 100;
	}
}

void nbody_quit()
{
	free(body);
}
