#ifndef SUDOKU_STATE_H
#define SUDOKU_STATE_H

typedef struct edge
{
	int target;
	struct edge *next;
} edge;

void free_edge(edge*);

struct state_role
{
	int num_options;

	union
	{
		int actor; // om num_options == 0, innehåller denna tillsatta rollen. :)
		edge *options;
	};
};

class state_ptr;

class sudoku_state
{
public:
    sudoku_state();
    sudoku_state(const sudoku_state &s); // Create a deep copy of a state
    ~sudoku_state();

    void remove_actor(int role, int actor);
    void assign_role(int role, int actor);
    void update_state_flag();

	int total_options;
	int status;

	int assigned_roles;
	state_role *roles;

	int *actors_rolecount;
	int num_super; // antalet superskådespelare som används
	int num_actors; // antalet vanliga skådespelare som används

    sudoku_state* incref()
    {
        refcount++;
        return this;
    }
    sudoku_state* incref(int refs)
    {
        refcount += refs;
        return this;
    }

    void decref()
    {
        refcount--;
        if (refcount <= 0)
            delete this;
    }
    bool refcountisone()
    {
        return refcount == 1;
    }
private:
	int refcount;
};

class state_ptr
{
private:
    sudoku_state *s;
public:
    state_ptr(const state_ptr &ptr)
    {
        s = ptr.s;
        s->incref();
    }
    ~state_ptr()
    {
        s->decref();
        s = 0;
    }
    sudoku_state* operator ->()
    {
        return s;
    }
};

#endif
