#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

int readint()
{
	register int num = 0;

	register char c = getchar();

	while(c < '0' || c > '9')
		c = getchar();

	do
	{
		num = num*10 + c - '0';
		c = getchar();
	} while(c >= '0' && c <= '9');

	return num;
}

#include "adkcasting.h"

void print_best();
void print_output(sudoku_state *s);
void sudoku_casting();
void update_state_flag(sudoku_state *s);
void free_edge(edge *e);
sudoku_state* first_state();
sudoku_state* simple_heuristic();
sudoku_state* dijkstra_heuristic();

void *malloc_fail(int size)
{
	void *ptr = malloc(size);
	if(ptr == NULL)
		exit(42);
	return ptr;
}
#define malloc malloc_fail

void *calloc_fail(int num, int size)
{
	void *ptr = calloc(num, size);
	if(ptr == NULL)
		exit(24);
	return ptr;
}
#define calloc calloc_fail

production prod;

void print_graph()
{
	puts("nodes");
	int i, j;
	for(i = 1; i <= prod.num_roles; ++i)
	{
		printf("%d:", i);

		for(j = 0; j < prod.roles[i].num_actors; ++j)
			printf(" %d", prod.roles[i].actors[j]);
		putchar('\n');
	}

	puts("neighbours");
	for(i = 1; i <= prod.num_roles; ++i)
	{
		printf("%d (%d):", i, prod.roles[i].num_edges);

		edge *e;
		for(e = prod.roles[i].first_edge; e != NULL; e = e->next)
			printf(" %d", e->target);
		putchar('\n');
	}
}

int main()
{
	//Utdataformat:
	//Rad ett: n, s och k (antal roller n≥1, antal scener s≥1, och antal skådespelare k≥2).

	prod.num_roles = readint();
	prod.num_scenes = readint();
	prod.num_actors = readint();

	//n rader: antal skådespelare som kan ansätta rollen + de skådespelare som kan ansätta rollen

	prod.roles = (role *)malloc(prod.num_roles * sizeof(role))-1;

	int i, j;

	for(i = 1; i <= prod.num_roles; ++i)
	{
		prod.roles[i].num_actors = readint();
		prod.roles[i].num_edges = 0;
		prod.roles[i].first_edge = NULL;

		prod.roles[i].actors = (int*)malloc(prod.roles[i].num_actors * sizeof(int));

		int j;
		for(j = 0; j < prod.roles[i].num_actors; ++j)
			prod.roles[i].actors[j] = readint();
	}


	//s rader: antal roller i scenen + de roller som ska vara med i scenen
	int *sceneroles = (int*)malloc(prod.num_roles * sizeof(int));

	for(i = 0; i < prod.num_scenes; ++i)
	{
		int r = readint();

		for(j = 0; j < r; ++j)
			sceneroles[j] = readint();

		for(j = 0; j < r; ++j)
		{
			int k;
			for(k = j+1; k < r; ++k)
			{
				int from = sceneroles[j];
				int to = sceneroles[k];
				edge *e = prod.roles[from].first_edge;

				for(e = prod.roles[from].first_edge; e != NULL; e = e->next)
				{
					if(e->target == to)
						break;
				}

				if(e != NULL)
					continue;

				edge *e_from, *e_to;

				e_from = (edge*)malloc(sizeof(edge));
				e_to = (edge*)malloc(sizeof(edge));

				e_from->next = prod.roles[from].first_edge;
				prod.roles[from].first_edge = e_from;

				e_to->next = prod.roles[to].first_edge;
				prod.roles[to].first_edge = e_to;

				e_from->target = to;
				e_to->target = from;

				++prod.roles[from].num_edges;
				++prod.roles[to].num_edges;
			}
		}
	}

	//print_graph();

	//sudoku_casting();
	srand( time(NULL) );
	int best_run = 0;
	sudoku_state *sbest = simple_heuristic();
	for(i = 0; i < 10; ++i)
	{
		sudoku_state *s = simple_heuristic();
		if (sbest == NULL)
		{
            sbest = s;
            continue;
		}

        if (s == NULL)
            continue;

		if (s->num_actors+s->num_super < sbest->num_actors+sbest->num_super)
		{
		    sbest->decref();
			sbest = s;
			best_run = i+1;
		}
		else
            s->decref();
	}
	/*
	fprintf(stderr, "starting dijkstra heuristic\n");
    for(i = 0; i < 10; ++i)
	{
		sudoku_state *s = dijkstra_heuristic();
        if (sbest == NULL)
		{
            sbest = s;
            continue;
		}

        if (s == NULL)
            continue;

		if (s->num_actors+s->num_super < sbest->num_actors+sbest->num_super)
		{
		    sbest->decref();
			sbest = s;
			best_run = i+11;
		}
		else
            s->decref();
	}*/

    if (sbest == NULL)
    {
        printf("no solution found\n");
        exit(5);
    }
    else
    {
        fprintf(stderr, "printing best solution (run %d)\n", best_run);
        print_output(sbest);
    }

/*
	for(i = 1; i <= n; ++i)
		free(roles[i].actors);

	for(i = 1; i <= n; ++i)
		free_edge(roles[i].first_edge);

	free(sceneroles);

	free(roles+1);
*/
	return 0;
}

sudoku_state *clone_state(sudoku_state *s)
{
	sudoku_state *new_state = new sudoku_state(*s);
	return new_state;
}

void print_state(sudoku_state *s)
{
	puts("state");
	int i;
	for(i = 1; i <= prod.num_roles; ++i)
	{
		printf("%d (%d):", i, s->roles[i].num_options);

		if(s->roles[i].num_options == 0)
		{
			printf(" actor: %d\n", s->roles[i].actor);
			continue;
		}

		edge *e;
		for(e = s->roles[i].options; e != NULL; e = e->next)
			printf(" %d", e->target);
		putchar('\n');
	}

	if(s->status & STATUS_INVALID)
		puts("åt helvete");
	else if((s->status & STATUS_SUCCESS) == STATUS_SUCCESS)
		puts("klar (:");
	else if(s->status & STATUS_DONE)
		puts("misslyckat att tillsätta divorna");
	else if((s->status & STATUS_DIVAS_ASSIGNED) == STATUS_DIVAS_ASSIGNED)
		puts("båda divorna tillsatta");
	else if(s->status & STATUS_DIVA1_ASSIGNED)
		puts("diva 1 tillsatt");
	else if(s->status & STATUS_DIVA2_ASSIGNED)
		puts("diva 2 tillsatt");
	else
		puts("inte klar");

}

void free_edge(edge *e)
{
	if(e->next != NULL)
		free_edge(e->next);
	free(e);
}

// skapa en state med alla självklara val gjorda
sudoku_state *first_state()
{
	sudoku_state *s = new sudoku_state();

	int i;
	for(i = 1; i <=prod.num_roles; ++i)
	{
        s->roles[i].prev = 0;
        s->roles[i].next = 0;

		s->roles[i].num_options = prod.roles[i].num_actors;
		edge *new_e = s->roles[i].options = (edge*)malloc(sizeof(edge));

		int j;
		for(j = 0; j < prod.roles[i].num_actors; ++j)
		{
			new_e->target = prod.roles[i].actors[j];
			if(j+1 == prod.roles[i].num_actors)
				break;
			new_e->next = (edge*)malloc(sizeof(edge));
			new_e = new_e->next;
		}
		new_e->next = NULL;
	}

	for(i = 1; i <= prod.num_roles; ++i)
	{
		if(s->roles[i].num_options == 1)
			s->assign_role(i, s->roles[i].options[0].target);
		if(s->status & STATUS_DONE)
			break;
	}

	s->update_state_flag();

	return s;
}

int num_states;

int min_score, max_score;

int best_score = 0x7FFFFFFF;
sudoku_state *best_state = NULL;

void score_state(sudoku_state *s)
{
	int score = prod.num_roles - s->assigned_roles + s->num_actors + s->num_super;
	if(score < best_score)
	{
		if(best_state != NULL)
		{
			best_state->decref();
		}
		best_score = score;
		best_state = s;
		s->incref();
	}
}

choice **sorted_choices;
int num_choices = 0;
int max_choices = 10;

void gen_choices(sudoku_state *s)
{
	s->update_state_flag();
	int score = prod.num_roles - s->assigned_roles + s->num_actors + s->num_super;

	if((s->status & STATUS_SUCCESS) == STATUS_SUCCESS)
	{
		score_state(s);
		return;
	}
	if(s->status & STATUS_DONE)
		return;

	if(min_score > score)
		min_score = score;
	if(max_score < score)
		max_score = score;

	// Hitta rollen med minst alternativ
	// Skapa ett choice för varje alternativ
	// Om vi hittar en roll med bara 2 alternativ tar vi den på en gång

	int least_options = 0x7FFFFFFF; // massor (:
	int idx = -1;

	int i;
	for(i = 1; i <= prod.num_roles; ++i)
	{
		int opts = s->roles[i].num_options;

		if(opts != 0 && opts < least_options)
		{
			idx = i;
			least_options = opts;
			if(opts == 2)
				break;
		}
	}

	assert(idx != -1);

	//least_options = 1;
	// lägg in alla alternativ i den sorterade listan av choices
	edge *e = s->roles[idx].options;

	choice *c_next = sorted_choices[score];

	choice *c;

	for(i = 0; i < least_options; ++i)
	{
		c = (choice*)malloc(sizeof(choice));
		c->next = c_next;
		c_next = c;
		c->state = s;
		c->role = idx;

		c->actor = e->target;
		e = e->next;

        s->incref();

		num_choices++;
		if (num_choices >= max_choices)
            break;
	}
	sorted_choices[score] = c;
}

sudoku_state *run_choice(choice *c)
{
	// klona state

	sudoku_state *s;
	if(c->state->refcountisone())
		s = c->state;
	else
	{
		s = clone_state(c->state);
		c->state->decref();
	}

	// anropa assign_role
	s->assign_role(c->role, c->actor);

	// uppdatera state
	s->update_state_flag();
	return s;
}

int num_assigned[0xFFFF];
edge *assigned[0xFFFF];
edge edgepool[0xFFFF];
int edge_index = 0;

void print_output(sudoku_state *s)
{
	// Utdataformat:
	// Rad ett: antal skådespelare som fått roller

	int assigned_roles = 0;

	int i;
	for(i = 1; i <= prod.num_roles; ++i)
	{
	    if (s->roles[i].num_options != 0)
	    {
	        fprintf(stderr, "role %d is unassigned in output\n", i);
            continue;
	    }

		int actor = s->roles[i].actor;

		edge *e = &edgepool[edge_index++];
		e->target = i;

		if(assigned[actor] == NULL)
			assigned_roles++;

		e->next = assigned[actor];
		assigned[actor] = e;

		num_assigned[actor]++;
	}

	printf("%d\n", assigned_roles);

	// En rad för varje skådespelare (som fått roller) med skådespelarens nummer,
	// antalet roller skådespelaren tilldelats samt numren på dessa roller
	for(i = 1; i <= prod.num_actors; ++i)
	{
		if(num_assigned[i] == 0)
			continue;
		printf("%d %d", i, num_assigned[i]);

		edge *e;
		for(e = assigned[i]; e != NULL; e = e->next)
			printf(" %d", e->target);
		putchar('\n');
	}
}

void print_best()
{
	if(best_state == NULL)
		exit(13);
	// skapa supers
	int i;
	for(i = 1; i <= prod.num_roles; ++i)
	{
		if(best_state->roles[i].num_options != 0)
			best_state->roles[i].actor = ++prod.num_actors;
	}
	print_output(best_state);
}

void assign_second(sudoku_state *s)
{
	if(s->status & STATUS_DIVA2_ASSIGNED)
	{
		s->update_state_flag();
		gen_choices(s);
		score_state(s);
		return;
	}

	int i;
	for(i = 1; i <= prod.num_roles; ++i)
	{
		edge *e;

		if(s->roles[i].num_options == 0)
			continue;

		for(e = s->roles[i].options; e != NULL; e = e->next)
		{
			if(e->target == 2)
			{
				sudoku_state *s_new = clone_state(s);
				s_new->assign_role(i, 2);
				s_new->update_state_flag();
				gen_choices(s_new);
				score_state(s_new);
				s_new->decref();
				break;
			}
		}
	}
}

void assign_divas(sudoku_state *s)
{
	if(s->status & STATUS_DIVA1_ASSIGNED)
	{
		assign_second(s);
		return;
	}

	int i;
	for(i = 1; i <= prod.num_roles; ++i)
	{
		edge *e;

		if(s->roles[i].num_options == 0)
			continue;

		for(e = s->roles[i].options; e != NULL; e = e->next)
		{
			if(e->target == 1)
			{
				sudoku_state *s_new = clone_state(s);
				s_new->assign_role(i, 1);
				assign_second(s_new);
				s_new->decref();

				break;
			}
		}
	}
}

void post_fix_divas()
{
    // force the divas into the state
    // find a role to give to the first diva
    // find another role to give to the other diva

    // TODO: implement me!
}

void sudoku_casting()
{
	// Skapa ett state som beskriver ett graftillstånd att börja jobba på.
	// Alla entydiga lösningssteg utförs.
	sudoku_state *s = first_state();

	if((s->status & STATUS_SUCCESS) == STATUS_SUCCESS)
	{
		print_output(s);
		return;
	}

	sorted_choices = (choice**)calloc(prod.num_actors + prod.num_roles, sizeof(choice *));

	min_score = prod.num_roles + prod.num_actors-1;
	max_score = 0;

	// Se till att divorna blir tilldelade varsin roll
	assign_divas(s);


	// *stoppa in valen i dinmamma
	while(1)
	{
		// samla ihop de bästa gamla choices för ny körning
		choice *prev_choices = NULL;

		// ifall inga val är kvar, avsluta
		if(sorted_choices[min_score] == NULL)
			break;

		// plocka dom 10 bästa choices och testa
		// testa alla choices som finns i poolen, tag den/de bästa, släng resten
		int i;
		for(i=0;i<1;i++)
		{
			choice *c = sorted_choices[min_score];

			if(c == NULL)
				break;

			sorted_choices[min_score] = c->next;

			// vi är klara när det inte finns fler val

			while(sorted_choices[min_score] == NULL && min_score < max_score)
				min_score++;

			c->next = prev_choices;
			prev_choices = c;
		}
		// töm sorted_choices
		while(sorted_choices[min_score] != NULL)
		{
			choice *c = sorted_choices[min_score];
			sorted_choices[min_score] = c->next;

			c->state->decref();
			free(c);

			while(sorted_choices[min_score] == NULL && min_score < max_score)
				min_score++;
		}
        num_choices = 0;

		min_score = prod.num_roles + prod.num_actors-1;
		max_score = 0;

		// kör alla val
		while(prev_choices != NULL)
		{
			choice *next = prev_choices->next;
			sudoku_state *s = run_choice(prev_choices);

			free(prev_choices);

			gen_choices(s);
			s->decref();

			prev_choices = next;
		}
	}

	free(sorted_choices);

	print_best();
	s->decref();
	//unlink_state(best_state);
	//print_output(s);
}

sudoku_state* simple_heuristic()
{
	// Skapa ett state som beskriver ett graftillstånd att börja jobba på.
	// Alla entydiga lösningssteg utförs.

	sudoku_state *s = first_state();

    while (1)
    {
    next_assignment:
        s->update_state_flag();
        if((s->status & STATUS_SUCCESS) == STATUS_SUCCESS)
        {
            return s;
        }
        if(s->assigned_roles == prod.num_roles)
        {
            // Om vi har tillsatt alla roller utan att tillsätta divorna måste detta ordnas
            // ett sätt är att börja om men det vore bättre att byta ut andra skådespelare mot divor
            fprintf(stderr, "simple heuristic fail\n");
            s->decref();
            //s = first_state();
            return NULL;
        }
        // Hitta rollen med minst alternativ
        // Skapa ett choice för varje alternativ
        // Om vi hittar en roll med bara 2 alternativ tar vi den på en gång

        int least_options = 0x7FFFFFFF; // massor (:
        int idx = -1;

        int shuffle = rand();
        for(int i = 1; i <= prod.num_roles; ++i)
        {
            int i2 = (i+shuffle)%prod.num_roles + 1;
            int opts = s->roles[i2].num_options;

            if(opts != 0 && opts < least_options)
            {
                idx = i2;
                least_options = opts;
                if(opts == 2)
                    break;
            }
        }

        assert(idx != -1);

        // Om divorna inte är tilldelade ännu så prioritera dem
        if ((s->status & STATUS_DIVAS_ASSIGNED) != STATUS_DIVAS_ASSIGNED)
        {
            edge *e = s->roles[idx].options;
            while (e != NULL)
            {
                if ((s->status & STATUS_DIVA1_ASSIGNED) == 0  && e->target == 1
                    || (s->status & STATUS_DIVA2_ASSIGNED) == 0  && e->target == 2)
                {
                    s->assign_role(idx, e->target);
                    goto next_assignment;
                }
                e = e->next;
            }
        }

        // Prioritera skådespelare som redan är med i produktionen
        int num_prio = 0;
        // kolla hur många av alternativen som redan är med i produktionen
        for (edge *e = s->roles[idx].options; e != NULL; e = e->next)
        {
            if (s->actors_rolecount[e->target] > 0)
                num_prio++;
        }

        if (num_prio > 0)
        {
            // Gör ett slumpmässigt val bland dom som finns med
            int i = rand()%num_prio;
            for (edge *e = s->roles[idx].options; e != NULL; e = e->next)
            {
                if (s->actors_rolecount[e->target] > 0)
                {
                    if (i > 0)
                        --i; // inte utvald
                    else
                    {
                        // utvald
                        s->assign_role(idx, e->target);
                        goto next_assignment;
                    }
                }
            }
        }

        // Gör ett slumpmässigt val
        edge *e = s->roles[idx].options;
        for(int i = rand()%least_options; i > 0; --i)
        {
            e = e->next;
        }
        s->assign_role(idx, e->target);
    }
}

sudoku_state* dijkstra_heuristic()
{
	// Skapa ett state som beskriver ett graftillstånd att börja jobba på.
	// Alla entydiga lösningssteg utförs.
	sudoku_state *s = first_state();

    if((s->status & STATUS_SUCCESS) == STATUS_SUCCESS)
    {
        return s;
    }

    // Tilldela roller i grafsökningsordning
    // Där roller är noder och scener bildar kanter
    // Tillsätt roller i ordning; grannar med minst alternativ kvar först

    // Börja med att stoppa in någon av rollerna som har minst antal alternativ i prioritetskön
    // Plocka ut en icke tilldelad roll ur prioritetskön
    // Tilldela rollen
    // Stoppa alla grannar i kön (gör automatiskt i sudoku_state)

    // Prioritetskön hålls uppdaterad av sudoku_state

    s->queue_clear();
    while (1)
    {
        if((s->status & STATUS_SUCCESS) == STATUS_SUCCESS)
        {
            return s;
        }
        if(s->assigned_roles == prod.num_roles)
        {
            // Om vi har tillsatt alla roller utan att tillsätta divorna måste detta ordnas
            // ett sätt är att börja om men det vore bättre att byta ut andra skådespelare mot divor
            fprintf(stderr, "dijkstra heuristic fail\n");
            s->decref();
            //s = first_state();
            return NULL;
        }
        // Hitta rollen med minst alternativ
        // Skapa ett choice för varje alternativ
        // Om vi hittar en roll med bara 2 alternativ tar vi den på en gång

        int least_options = 0x7FFFFFFF; // massor (:
        int idx = -1;

        int shuffle = rand();
        for(int i = 1; i <= prod.num_roles; ++i)
        {
            int i2 = (i+shuffle)%prod.num_roles + 1;
            int opts = s->roles[i2].num_options;

            if(opts != 0 && opts < least_options)
            {
                idx = i2;
                least_options = opts;
                if(opts == 2)
                    break;
            }
        }

        assert(idx != -1);
        assert(s->queue_poll() == 0);

        s->queue_add(idx);
        for(idx = s->queue_pop(); idx > 0; idx = s->queue_pop())
        {
            // det borde inte hända att en tilldelad roll ligger i kön
            if (s->roles[idx].num_options == 0)
                continue;

            // Om divorna inte är tilldelade ännu så prioritera dem
            if ((s->status & STATUS_DIVAS_ASSIGNED) != STATUS_DIVAS_ASSIGNED)
            {
                edge *e = s->roles[idx].options;
                while (e != NULL)
                {
                    if ((s->status & STATUS_DIVA1_ASSIGNED) == 0  && e->target == 1
                        || (s->status & STATUS_DIVA2_ASSIGNED) == 0  && e->target == 2)
                    {
                        s->assign_role(idx, e->target);
                        break;;
                    }
                    e = e->next;
                }
            }
            if (s->roles[idx].num_options != 0)
            {
                // Prioritera skådespelare som redan är med i produktionen
                int num_prio = 0;
                // kolla hur många av alternativen som redan är med i produktionen
                for (edge *e = s->roles[idx].options; e != NULL; e = e->next)
                {
                    if (s->actors_rolecount[e->target] > 0)
                        num_prio++;
                }

                if (num_prio > 0)
                {
                    // Gör ett slumpmässigt val bland dom som finns med
                    int i = rand()%num_prio;
                    for (edge *e = s->roles[idx].options; e != NULL; e = e->next)
                    {
                        if (s->actors_rolecount[e->target] > 0)
                        {
                            if (i > 0)
                                --i; // inte utvald
                            else
                            {
                                s->assign_role(idx, e->target);
                                break;
                            }
                        }
                    }
                }
            }
            if (s->roles[idx].num_options != 0)
            {
                // Gör ett slumpmässigt val
                edge *e = s->roles[idx].options;
                for(int i = rand()%least_options; i > 0; --i)
                {
                    e = e->next;
                }
                s->assign_role(idx, e->target);
            }
        }
    }
}


