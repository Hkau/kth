#ifndef NBODY_H
#define NBODY_H

#include <stddef.h>

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

typedef struct
{
	float mass;
	vec3f pos;
	vec3f vel;
	vec3f force;
} body_t;

extern body_t* body;
extern size_t num_bodies;

static inline float sq(float x)
{
	return x * x;
}

extern float bounds;

extern float turbo;

#endif

