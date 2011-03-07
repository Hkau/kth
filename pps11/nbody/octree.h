#ifndef OCTREE_H
#define OCTREE_H

#include "nbody.h"

#ifndef NO_THREADS
#include <pthread.h>
#endif

typedef struct octree
{
	vec3f min, max;

	double mass; // float isn't enough to sum them up! :O
	vec3f center;
	int count;

	struct octree *parent;
	struct octree *children;

	body_t *bodies;

#ifndef NO_THREADS
	pthread_mutex_t lock;
#endif
} octree;

extern octree *top;

size_t oct_index(octree *node, vec3f pos);
void oct_init(float bounds);
void oct_quit();

void oct_add(octree *node, body_t *body);
void oct_move(body_t *body, vec3f pos);
octree *oct_getchild(octree *node, body_t *body);

#endif

