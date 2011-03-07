#ifndef NBODY_H
#define NBODY_H

#include <stddef.h>
#include <stdbool.h>

void nbody_init(int count);
void nbody_quit();

typedef struct
{
	float x, y, z;
} vec3f;

static inline vec3f vec3f_zero()
{
	vec3f v = {0, 0, 0};
	return v;
}
static inline vec3f vec3f_add(vec3f a, vec3f b)
{
	vec3f v = {a.x+b.x, a.y+b.y, a.z+b.z};
	return v;
}

static inline vec3f vec3f_sub(vec3f a, vec3f b)
{
	vec3f v = {a.x-b.x, a.y-b.y, a.z-b.z};
	return v;
}

static inline vec3f vec3f_scale(vec3f a, float lambda)
{
	vec3f v = {a.x * lambda, a.y * lambda, a.z * lambda};
	return v;
}

static inline vec3f vec3f_xyz(float x, float y, float z)
{
	vec3f v = {x, y, z};
	return v;
}

static inline float vec3f_len_sq(vec3f a)
{
	return a.x * a.x + a.y * a.y + a.z * a.z;
}

typedef struct body_t
{
	float mass;
	vec3f pos;
	vec3f vel;
	vec3f force;

#ifdef USE_OCTREE
	// next ptr used for "next" in octree
	struct body_t *next;
	struct octree *node;
#endif
} body_t;

extern body_t* body;
extern size_t num_bodies;
void body_move(body_t *body, vec3f pos);

static inline float sq(float x)
{
	return x * x;
}

extern float bounds;

extern float turbo;

extern bool allow_negative;

extern int num_steps;

extern float G;

static inline float min(float a, float b)
{
	if(a < b)
		return a;
	return b;
}

static inline float max(float a, float b)
{
	if(a > b)
		return a;
	return b;
}

extern void calc_forces();
extern void add_velocity();

extern void parse_args(int argc, char *argv[]);
extern void client_start();
extern void client_exit();

extern void body_move_pos(body_t *body, vec3f pos);

#endif

