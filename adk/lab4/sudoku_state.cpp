#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "sudoku_state.h"
#include "adkcasting.h"

extern production prod;

sudoku_state::sudoku_state()
{
    memset(this, 0, sizeof(sudoku_state));

    this->roles = (new state_role[prod.num_roles]) - 1;
    this->actors_rolecount = (new int[prod.num_actors]) - 1;

    refcount = 1;
}

// Create a deep copy of a state
sudoku_state::sudoku_state(const sudoku_state &s)
{
    // copy all values
    *this = s;

    // reset refcount
    this->refcount = 1;

    // create new copies of roles and actors_rolecount
    this->roles = (new state_role[prod.num_roles]) - 1;
    this->actors_rolecount = (new int[prod.num_actors]) - 1;
    memcpy(this->actors_rolecount+1, s.actors_rolecount+1, prod.num_actors * sizeof(int));

    for(int i = 1; i <= prod.num_roles; ++i)
    {
        this->roles[i].num_options = s.roles[i].num_options;
        if(s.roles[i].num_options == 0)
        {
            this->roles[i].actor = s.roles[i].actor;
            continue;
        }

        edge *new_e = this->roles[i].options = (edge*)malloc(sizeof(edge));

        edge *e;
        for(e = s.roles[i].options; ; e = e->next)
        {
            new_e->target = e->target;
            if(e->next == NULL)
                break;
            new_e->next = (edge*)malloc(sizeof(edge));
            new_e = new_e->next;
        }
        new_e->next = NULL;
    }
}

sudoku_state::~sudoku_state()
{
    // Deallocate all used memory
    for(int i = 1; i <= prod.num_roles; ++i)
    {
        if(roles[i].num_options != 0)
            free_edge(roles[i].options);
    }
    delete (roles+1);
    delete (actors_rolecount+1);
}

void sudoku_state::remove_actor(int role, int actor)
{
	// Iterera �ver alla m�jliga sk�despelare f�r rollen och stryk ev. f�rekomst av actor.
	// Om det bara finns ett alternativ kvar, tills�tt rollen rekursivt.
	// Om rollen saknar alternativ, skapa en supersk�dis, eller panika.

	if(roles[role].num_options == 0)
	{
		if(roles[role].actor == actor)
		{
			roles[role].actor = -1;
			status = STATUS_INVALID;
			num_super++;
		}
		return;
	}

	edge *opts = roles[role].options; // grannrollens m�jliga sk�despelare i denna state

	// Ta bort actor ifall actor f�rekommer som m�jlig sk�despelare till 'angr�nsande' roll
	if(opts->target == actor)
	{
		// Minska antalet m�jliga roller
		roles[role].num_options--;

		// Plocka bort actor som alternativ
		edge *e = roles[role].options;
		roles[role].options = opts->next;
		free(e);

		// Om rollen �r entydig
		if(roles[role].num_options == 1)
			// Tills�tt rollen
			assign_role(role, roles[role].options->target);

		// Rollen f�rdigbehandlad, forts�tt l�sa ev. andra konflikter
		return;
	}

	for(; opts->next != NULL; opts = opts->next)
	{
		if(opts->next->target == actor)
		{
			// ta bort f�rekomsten av actor
			roles[role].num_options--;
			edge *e = opts->next;
			opts->next = opts->next->next;
			free(e);

			// Om rollen �r entydig
			if(roles[role].num_options == 1)
				// Tills�tt rollen
				assign_role(role, roles[role].options->target);
			return;
		}
	}
}

void sudoku_state::assign_role(int role, int actor)
{
	assert(roles[role].num_options != 0);
	//if(roles[role].num_options == 0)
	//	*(int *)NULL = 42;

    // ta bort alla alternativ och best�m sk�despelare
	free_edge(roles[role].options);
	roles[role].num_options = 0;
	roles[role].actor = actor;
	assigned_roles ++;

	// R�kna antalet sk�despelare och antalet roller per sk�despelare
	if (actors_rolecount[actor] == 0)
		num_actors++;
	actors_rolecount[actor]++;

	// Stryk alla alternativ i konlikt med detta val och alla val som tvingas av detta
	// Iterera �ver varje kant, e, i statiska scengrafen som symboliserar att tv� roller deltar i
	// samma scen och stryk alla f�rekomster av sk�despelaren actor f�r att undvika konflikter.

	if(actor <= 2)
	{
		if(actor == 1)
			status |= STATUS_DIVA1_ASSIGNED;
		else
			status |= STATUS_DIVA2_ASSIGNED;

		if(assigned_roles == prod.num_roles)
		{
			status |= STATUS_DONE;
			return;
		}

		// jobbiga divor :(
		int other_actor = 3 - actor; // (actor == 1) ? 2 : 1;
		for(edge *e = prod.roles[role].first_edge; e != NULL; e = e->next)
		{
			remove_actor(e->target, other_actor);
			if(status & STATUS_DONE)
				return;
		}

		remove_actor(role, other_actor);
		if(status & STATUS_DONE)
			return;
	}

	if(assigned_roles == prod.num_roles)
	{
		status |= STATUS_DONE;
		return;
	}

	edge *e;
	for(e = prod.roles[role].first_edge; e != NULL; e = e->next)
	{
		remove_actor(e->target, actor);
		if(status & STATUS_DONE)
			return;
	}
}

void sudoku_state::update_state_flag()
{
	if(status & STATUS_DONE)
		return;

    /*
	for(int i = 1; i <= prod.num_roles; ++i)
	{
		if(roles[i].num_options != 0)
			return;
	}
	*/
	if (assigned_roles == prod.num_roles)
        status |= STATUS_DONE;
}
