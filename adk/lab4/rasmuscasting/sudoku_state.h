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
		int actor; // om num_options == 0, inneh�ller denna tillsatta rollen. :)
		edge *options;
	};

	// Prioritetsk�
    // En dubbell�nkad lista f�r varje prioritetsniv�
    // varje roll har ett eget element med tv� pekare
    // n�r en roll uppdaterar sin lista av sk�despelare flyttas den i prioritetsk�n
    // n�r en roll tilldelas tas den bort ur prioritetsk�n

	int next; // n�sta roll p� denna niv�
	int prev; // f�reg�ende roll p� denna niv�
	int in_queue;
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
	int num_super; // antalet supersk�despelare som anv�nds
	int num_actors; // antalet vanliga sk�despelare som anv�nds

    int *queue;
    int min_prio;
    int max_prio;

    int queue_poll()
    {
        // Tighten the limits
        while (min_prio <= max_prio && queue[min_prio] == 0)
            min_prio++;
        while (min_prio <= max_prio && queue[max_prio] == 0)
            max_prio--;
        if (min_prio > max_prio)
        {
            min_prio = 0x7FFFFFFF;
            max_prio = 0;
            return 0;
        }

        // return minimum in queue
        return queue[min_prio];
    }

    int queue_pop()
    {
        int role = queue_poll();
        if (role != 0)
        {
            int next = roles[role].next;
            roles[role].prev = 0;
            roles[role].next = 0;
            roles[role].in_queue = 0;
            queue[min_prio] = next;
        }
        return role;
    }

    void queue_clear();

    void queue_remove(int role)
    {
        // kolla f�rst om rollen �r med i k�n
        if (roles[role].in_queue == 0)
            return;

        int p = roles[role].num_options;

        // plocka ut rollen ur k�n och lappa ihop den l�nkade listan
        if (roles[role].prev == 0)
            queue[p] = roles[role].next;
        else
            roles[roles[role].prev].next = roles[role].next;

        if (roles[role].next != 0)
            roles[roles[role].next].prev = roles[role].prev;

        roles[role].prev = 0;
        roles[role].next = 0;
        roles[role].in_queue = 0;
    }

    void queue_add(int role)
    {
        if (roles[role].in_queue != 0)
            return;

        // priority is number of options
        int p = roles[role].num_options;

        //if (!(roles[role].prev == 0 && roles[role].next == 0 && queue[p] != role))
        //    *((int*)42) = 11;

        assert(p != 0);

        // l�gg bara till obesatta roller
        if (p == 0)
            return;

        // l�gg till rollen f�rst i k�n
        roles[role].prev = 0;
        roles[role].next = queue[p];
        roles[queue[p]].prev = role;
        queue[p] = role;

        roles[role].in_queue = 1;

        // flytta gr�nserna
        if (p < min_prio)
            min_prio = p;
        if (p > max_prio)
            max_prio = p;
    }

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
