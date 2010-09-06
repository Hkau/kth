#ifndef ADKCASTING_H
#define ADKCASTING_H

#include "sudoku_state.h"

const int STATUS_DONE = 1;
const int STATUS_DIVA1_ASSIGNED = 2;
const int STATUS_DIVA2_ASSIGNED = 4;
const int STATUS_DIVAS_ASSIGNED = STATUS_DIVA1_ASSIGNED | STATUS_DIVA2_ASSIGNED;
const int STATUS_SUCCESS = STATUS_DONE | STATUS_DIVAS_ASSIGNED;
const int STATUS_INVALID = 8;

struct role
{
	int assigned_actor;

	int num_actors;
	int *actors;

	int num_edges;
	edge *first_edge;
};

struct choice
{
	sudoku_state *state;
	int role;
	int actor;

	struct choice *next;
};

struct production
{
    int num_roles;
    int num_scenes;
    int num_actors;
    role *roles;
};

#endif
