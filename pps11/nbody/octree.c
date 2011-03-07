#include "octree.h"
#include <stdlib.h>
#include <assert.h>

#define MIN_NODE_SIZE 10.f
#define SPLIT_LIMIT 4
octree *top = NULL;

void oct_init(float bounds)
{
	top = calloc(sizeof(octree), 1);
#ifndef NO_THREADS
	pthread_mutex_init(&top->lock, NULL);
#endif

	top->min.x = top->min.y = top->min.z = -bounds;
	top->max.x = top->max.y = top->max.z = bounds;
}

size_t oct_index(octree *node, vec3f pos)
{
	return ((pos.x > (node->max.x + node->min.x) / 2) << 2) |
	       ((pos.y > (node->max.y + node->min.y) / 2) << 1) |
	        (pos.z > (node->max.z + node->min.z) / 2);
}

octree *oct_getchild(octree *node, body_t *body)
{
	return node->children == NULL ? NULL : &node->children[oct_index(node, body->pos)];
}

void oct_split(octree *node)
{
	node->children = calloc(sizeof(octree), 8);
	int i;
	for(i = 0; i < 8; ++i)
	{
		node->children[i].parent = node;
#ifndef NO_THREADS
		pthread_mutex_init(&node->children[i].lock, NULL);
#endif
	}

	float middle = (node->max.x - node->min.x) / 2;

	// create 8 children based on oct_index's index
	node->children[0].min = node->min;
	node->children[1].min = vec3f_add(node->min, vec3f_xyz(0, 0, middle));
	node->children[2].min = vec3f_add(node->min, vec3f_xyz(0, middle, 0));
	node->children[3].min = vec3f_add(node->min, vec3f_xyz(0, middle, middle));
	node->children[4].min = vec3f_add(node->min, vec3f_xyz(middle, 0, 0));
	node->children[5].min = vec3f_add(node->min, vec3f_xyz(middle, 0, middle));
	node->children[6].min = vec3f_add(node->min, vec3f_xyz(middle, middle, 0));
	node->children[7].min = vec3f_add(node->min, vec3f_xyz(middle, middle, middle));

	for(i = 0; i < 8; ++i)
		node->children[i].max = vec3f_add(node->children[i].min, vec3f_xyz(middle, middle, middle));

	body_t *b, *next;
	for(b = node->bodies; b != NULL; b = next)
	{
		next = b->next;
		oct_add(oct_getchild(node, b), b);
	}
}

void oct_add(octree *node, body_t *body)
{
#ifndef NO_THREADS
	pthread_mutex_lock(&node->lock);
#endif
	while(node->children != NULL)
	{
		node->count++;
		node->mass += body->mass;
		node->center = vec3f_add(node->center, body->pos);
#ifndef NO_THREADS
		pthread_mutex_unlock(&node->lock);
#endif
		node = oct_getchild(node, body);
#ifndef NO_THREADS
		pthread_mutex_lock(&node->lock);
#endif

	}

	while(node->count >= SPLIT_LIMIT && (node->max.x - node->min.x > MIN_NODE_SIZE))
	{
		node->count++;
		node->mass += body->mass;
		node->center = vec3f_add(node->center, body->pos);
		oct_split(node);
#ifndef NO_THREADS
		pthread_mutex_unlock(&node->lock);
#endif
		node = oct_getchild(node, body);
#ifndef NO_THREADS
		pthread_mutex_lock(&node->lock);
#endif
	}

	// add here

	node->count++;
	node->mass += body->mass;
	node->center = vec3f_add(node->center, body->pos);

	body->node = node;
	body->next = node->bodies;
	node->bodies = body;

#ifndef NO_THREADS
	pthread_mutex_unlock(&node->lock);
#endif
}

void oct_move(body_t *body, vec3f pos)
{
	body->pos = pos;

	octree *node = body->node;

#ifndef NO_THREADS
	while(true)
	{
		pthread_mutex_lock(&node->lock);
		// make sure body wasn't moved while getting lock
		if(body->node == node)
			break;
		//printf("I can't believe it helped!\n");
		pthread_mutex_unlock(&node->lock);
		node = body->node;
		// otherwise, keep getting lock further down,
		// until we've acquired the correct lock.
	}
#endif

	if(pos.x >= node->min.x && pos.x <= node->max.x &&
	   pos.y >= node->min.y && pos.y <= node->max.y &&
	   pos.z >= node->min.z && pos.z <= node->max.z)
	{
#ifndef NO_THREADS
	pthread_mutex_unlock(&node->lock);
#endif

		return; // already in right oct..
	}

	// unlink body from node, so that we can move higher up in the tree
	body_t *b;
	if(node->bodies == body)
		node->bodies = body->next;
	else
	{
		for(b = node->bodies; ; b = b->next)
		{
			if(b->next == body)
			{
				b->next = body->next;
				break;
			}
		}
	}

	while(1)
	{
		node->count--;
		node->mass -= body->mass;
		if(node->count == 0)
		{
		//	if(node->mass != 0)
		//		printf("mass nonzero: %f\n", node->mass);
			assert(node->mass == 0);
		}
		node->center = vec3f_sub(node->center, body->pos);
#ifndef NO_THREADS
		pthread_mutex_unlock(&node->lock);
#endif
		if(pos.x >= node->min.x && pos.x <= node->max.x &&
		   pos.y >= node->min.y && pos.y <= node->max.y &&
		   pos.z >= node->min.z && pos.z <= node->max.z)
			break; // in right oct..

		node = node->parent;
		if(node == NULL)
			break;
#ifndef NO_THREADS
		pthread_mutex_lock(&node->lock);
#endif
	}
	if(node == NULL)
		oct_add(top, body);
	else
		oct_add(node, body);
}

void oct_free(octree *node)
{
	if(node->children != NULL)
	{
		int i;
		for(i = 0; i < 8; ++i)
		{
			oct_free(&node->children[i]);
		}
		free(node->children);
	}

#ifndef NO_THREADS
	pthread_mutex_destroy(&node->lock);
#endif
}

void oct_quit()
{
	oct_free(top);
	free(top);
	top = NULL;
}

