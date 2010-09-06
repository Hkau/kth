#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "sudoku_state.h"
#include "adkcasting.h"

extern production prod;

sudoku_state::sudoku_state()
{
    memset(this, 0, sizeof(sudoku_state));

    roles = new state_role[prod.num_roles+1];
    memset(roles+1, 0, prod.num_roles*sizeof(state_role));

    actors_rolecount = (new int[prod.num_actors]) - 1;
    memset(actors_rolecount+1, 0, prod.num_actors*sizeof(int));

    refcount = 1;

    queue = new int[prod.num_roles];
    memset(queue, 0, prod.num_roles*sizeof(int));

    min_prio = 0x7FFFFFFF;
    max_prio = 0;
}

// Create a deep copy of a state
sudoku_state::sudoku_state(const sudoku_state &s)
{
    // copy all values
    *this = s;

    // reset refcount
    this->refcount = 1;

    // create new copies of roles and actors_rolecount
    this->roles = new state_role[prod.num_roles+1];
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

    // TODO: copy queue
}

sudoku_state::~sudoku_state()
{
    // Deallocate all used memory
    delete [] queue;
    for(int i = 1; i <= prod.num_roles; ++i)
    {
        if(roles[i].num_options != 0)
            free_edge(roles[i].options);
    }
    delete [] roles;
    delete [] (actors_rolecount+1);
}

void sudoku_state::queue_clear()
{
    for(int i = 1; i <= prod.num_roles; ++i)
    {
        roles[i].prev = 0;
        roles[i].next = 0;
        roles[i].in_queue = 0;
    }

    min_prio = 0x7FFFFFFF;
    max_prio = 0;
}

void sudoku_state::remove_actor(int role, int actor)
{
	// Iterera över alla möjliga skådespelare för rollen och stryk ev. förekomst av actor.
	// Om det bara finns ett alternativ kvar, tillsätt rollen rekursivt.
	// Om rollen saknar alternativ, skapa en superskådis, eller panika.

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

    // Plocka tillfälligt bort rollen ur prioritetskön för att flytta den till en annan nivå
    queue_remove(role);

	edge *opts = roles[role].options; // rollens möjliga skådespelare i denna state

	// Ta bort actor från listan av alternativ
	// specialfall för först platsen i listan
	if(opts->target == actor)
	{
		// Plocka bort actor som alternativ
		edge *e = roles[role].options;
		roles[role].options = opts->next;
		free(e);

        // Minska antalet möjliga roller
        roles[role].num_options--;

		// Om rollen är entydig
        if(roles[role].num_options == 0)
		{
			roles[role].actor = -1;
			status = STATUS_INVALID;
			num_super++;
			assigned_roles++;
		}
		else if(roles[role].num_options == 1)
			// Tillsätt rollen
			assign_role(role, roles[role].options->target);
        else
            // Lägg till rollen i prioritetskön igen, om den inte fanns i kön från början läggs den till nu
            queue_add(role);

		// actor är nu borta från listan och rollen är färdigbehandlad
		return;
	}

    // resterande platser i listan
	for(; opts->next != NULL; opts = opts->next)
	{
		if(opts->next->target == actor)
		{
			// ta bort förekomsten av actor
			roles[role].num_options--;
			edge *e = opts->next;
			opts->next = opts->next->next;
			free(e);

            // Minska antalet möjliga roller
            roles[role].num_options--;

			// Om rollen är entydig
            if(roles[role].num_options == 0)
            {
                roles[role].actor = -1;
                status = STATUS_INVALID;
                num_super++;
                assigned_roles++;
            }
            else if(roles[role].num_options == 1)
				// Tillsätt rollen
				assign_role(role, roles[role].options->target);
            else
                // Lägg till rollen i prioritetskön igen, om den inte fanns i kön från början läggs den till nu
                queue_add(role);

			return;
		}
	}
}

void sudoku_state::assign_role(int role, int actor)
{
	assert(roles[role].num_options != 0);
	//if(roles[role].num_options == 0)
	//	*(int *)NULL = 42;

    // ta bort rollen ur prioritetskön
    queue_remove(role);

    // ta bort alla alternativ och bestäm skådespelare
	free_edge(roles[role].options);
	roles[role].num_options = 0;
	roles[role].actor = actor;
	assigned_roles++;

	// Räkna antalet skådespelare och antalet roller per skådespelare
	if (actors_rolecount[actor] == 0)
		num_actors++;
	actors_rolecount[actor]++;

	// Stryk alla alternativ i konlikt med detta val och alla val som tvingas av detta
	// Iterera över varje kant, e, i statiska scengrafen som symboliserar att två roller deltar i
	// samma scen och stryk alla förekomster av skådespelaren actor för att undvika konflikter.

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
