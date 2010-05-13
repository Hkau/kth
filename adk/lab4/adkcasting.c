#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline int readint()
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

void print_best();

typedef struct edge
{
	int target;

	struct edge *next;
} edge;

void free_edge(edge *e);

typedef struct
{
	int assigned_actor;

	int num_actors;
	int *actors;

	int num_edges;
	edge *first_edge;
} role;

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

int n, s, k;

role *roles;

void print_graph()
{
	puts("nodes");
	int i, j;
	for(i = 1; i <= n; ++i)
	{
		printf("%d:", i);
		
		for(j = 0; j < roles[i].num_actors; ++j)
			printf(" %d", roles[i].actors[j]);
		putchar('\n');
	}

	puts("neighbours");
	for(i = 1; i <= n; ++i)
	{
		printf("%d (%d):", i, roles[i].num_edges);

		edge *e;
		for(e = roles[i].first_edge; e != NULL; e = e->next)
			printf(" %d", e->target);
		putchar('\n');
	}
}

void sudoku_casting();

int main()
{
	//Utdataformat:
	//Rad ett: n, s och k (antal roller n≥1, antal scener s≥1, och antal skådespelare k≥2).

	n = readint();
	s = readint();
	k = readint();

	//n rader: antal skådespelare som kan ansätta rollen + de skådespelare som kan ansätta rollen

	roles = (role *)malloc(n * sizeof(role))-1;

	int i, j;

	for(i = 1; i <= n; ++i)
	{
		roles[i].num_actors = readint();
		roles[i].num_edges = 0;
		roles[i].first_edge = NULL;

		roles[i].actors = malloc(roles[i].num_actors * sizeof(int));

		int j;
		for(j = 0; j < roles[i].num_actors; ++j)
			roles[i].actors[j] = readint();
	}


	//s rader: antal roller i scenen + de roller som ska vara med i scenen
	int *sceneroles = malloc(n * sizeof(int));

	for(i = 0; i < s; ++i)
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
				edge *e = roles[from].first_edge;

				for(e = roles[from].first_edge; e != NULL; e = e->next)
				{
					if(e->target == to)
						break;
				}

				if(e != NULL)
					continue;

				edge *e_from, *e_to;

				e_from = malloc(sizeof(edge));
				e_to = malloc(sizeof(edge));

				e_from->next = roles[from].first_edge;
				roles[from].first_edge = e_from;

				e_to->next = roles[to].first_edge;
				roles[to].first_edge = e_to;

				e_from->target = to;
				e_to->target = from;

				++roles[from].num_edges;
				++roles[to].num_edges;
			}
		}
	}

	//print_graph();

	sudoku_casting();
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

typedef struct
{
	int num_options;

	union
	{
		int actor; // om num_options == 0, innehåller denna tillsatta rollen. :)
		edge *options;
	};
} state_role;

enum status_flag
{
	STATUS_DONE = 1,
	STATUS_DIVA1_ASSIGNED = 2,
	STATUS_DIVA2_ASSIGNED = 4,
	STATUS_DIVAS_ASSIGNED = STATUS_DIVA1_ASSIGNED | STATUS_DIVA2_ASSIGNED,
	STATUS_SUCCESS = STATUS_DONE | STATUS_DIVAS_ASSIGNED,
	STATUS_INVALID = 8
};

typedef struct
{
	int total_options;
	enum status_flag status;
	int refcount;

	int assigned_roles;
	state_role *roles;

	int *actors_rolecount;
	int num_super; // antalet superskådespelare som används
	int num_actors; // antalet vanliga skådespelare som används
} sudoku_state;

sudoku_state *clone_state(sudoku_state *s)
{
	sudoku_state *new_state = calloc(1,sizeof(sudoku_state));

	*new_state = *s;
	new_state->refcount = 1;

	new_state->roles = (state_role *) malloc(n * sizeof(state_role)) - 1;
	new_state->actors_rolecount = (int*) malloc(k * sizeof(int)) - 1;
	memcpy(new_state->actors_rolecount+1, s->actors_rolecount+1, k * sizeof(int));

	int i;
	for(i = 1; i <= n; ++i)
	{
		new_state->roles[i].num_options = s->roles[i].num_options;
		if(s->roles[i].num_options == 0)
		{
			new_state->roles[i].actor = s->roles[i].actor;
			continue;
		}

		edge *new_e = new_state->roles[i].options = malloc(sizeof(edge));

		edge *e;
		for(e = s->roles[i].options; ; e = e->next)
		{
			new_e->target = e->target;
			if(e->next == NULL)
				break;
			new_e->next = malloc(sizeof(edge));
			new_e = new_e->next;
		}
		new_e->next = NULL;
	}
	return new_state;
}


void print_state(sudoku_state *s)
{
	puts("state");
	int i;
	for(i = 1; i <= n; ++i)
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

void free_state(sudoku_state *s);
void assign_role(sudoku_state *s, int role, int actor);

void unlink_state(sudoku_state *s)
{
	s->refcount --;
	if(s->refcount == 0)
		free_state(s);
}

void remove_actor(sudoku_state *s, int role, int actor)
{
	// Iterera över alla möjliga skådespelare för rollen och stryk ev. förekomst av actor.
	// Om det bara finns ett alternativ kvar, tillsätt rollen rekursivt.
	// Om rollen saknar alternativ, skapa en superskådis, eller panika.

	if(s->roles[role].num_options == 0)
	{
		if(s->roles[role].actor == actor)
		{
			s->roles[role].actor = -1;
			s->status = STATUS_INVALID;
			s->num_super++;
		}
		return;
	}

	edge *opts = s->roles[role].options; // grannrollens möjliga skådespelare i denna state

	// Ta bort actor ifall actor förekommer som möjlig skådespelare till 'angränsande' roll
	if(opts->target == actor)
	{
		// Minska antalet möjliga roller
		s->roles[role].num_options--;

		// Plocka bort actor som alternativ
		edge *e = s->roles[role].options;
		s->roles[role].options = opts->next;
		free(e);

		// Om rollen är entydig
		if(s->roles[role].num_options == 1)
			// Tillsätt rollen
			assign_role(s, role, s->roles[role].options->target);

		// Rollen färdigbehandlad, fortsätt lösa ev. andra konflikter
		return;
	}

	for(; opts->next != NULL; opts = opts->next)
	{
		if(opts->next->target == actor)
		{
			// ta bort förekomsten av actor
			s->roles[role].num_options--;
			edge *e = opts->next;
			opts->next = opts->next->next;
			free(e);

			// Om rollen är entydig
			if(s->roles[role].num_options == 1)
				// Tillsätt rollen
				assign_role(s, role, s->roles[role].options->target);
			return;
		}
	}
}

#include <assert.h>
void assign_role(sudoku_state *s, int role, int actor)
{
	assert(s->roles[role].num_options != 0);
	//if(s->roles[role].num_options == 0)
	//	*(int *)NULL = 42;

	s->assigned_roles ++;
	s->roles[role].actor = actor;
	s->roles[role].num_options = 0;

	// Räkna antalet skådespelare och antalet roller per skådespelare
	if (s->actors_rolecount[actor] == 0)
		s->num_actors++;
	s->actors_rolecount[actor]++;

	// Stryk alla alternativ i konlikt med detta val och alla val som tvingas av detta
	// Iterera över varje kant, e, i statiska scengrafen som symboliserar att två roller deltar i
	// samma scen och stryk alla förekomster av skådespelaren actor för att undvika konflikter.

	if(actor <= 2)
	{
		if(actor == 1)
			s->status |= STATUS_DIVA1_ASSIGNED;
		else
			s->status |= STATUS_DIVA2_ASSIGNED;

		if(s->assigned_roles == n)
		{
			s->status |= STATUS_DONE;
			return;
		}

		// jobbiga divor :(
		edge *e;
		int other_actor = 3 - actor; // (actor == 1) ? 2 : 1;
		for(e = roles[role].first_edge; e != NULL; e = e->next)
		{
			remove_actor(s, e->target, other_actor);
			if(s->status & STATUS_DONE)
				return;
		}

		remove_actor(s, role, other_actor);
		if(s->status & STATUS_DONE)
			return;
	}

	if(s->assigned_roles == n)
	{
		s->status |= STATUS_DONE;
		return;
	}

	edge *e;
	for(e = roles[role].first_edge; e != NULL; e = e->next)
	{
		remove_actor(s, e->target, actor);
		if(s->status & STATUS_DONE)
			return;
	}
}

void update_state_flag(sudoku_state *s);

// skapa en state med alla självklara val gjorda
sudoku_state *first_state()
{
	sudoku_state *s = calloc(1, sizeof(sudoku_state));

	s->roles = (state_role *)malloc(n*sizeof(state_role)) - 1;
	s->actors_rolecount = (int*)calloc(k, sizeof(int)) - 1;

	s->refcount = 1;

	int i;
	for(i = 1; i <=n; ++i)
	{
		s->roles[i].num_options = roles[i].num_actors;
		edge *new_e = s->roles[i].options = malloc(sizeof(edge));

		int j;
		for(j = 0; j < roles[i].num_actors; ++j)
		{
			new_e->target = roles[i].actors[j];
			if(j+1 == roles[i].num_actors)
				break;
			new_e->next = malloc(sizeof(edge));
			new_e = new_e->next;
		}
		new_e->next = NULL;
	}

	for(i = 1; i <= n; ++i)
	{
		if(roles[i].num_actors == 1)
			assign_role(s, i, roles[i].actors[0]);
		if(s->status & STATUS_DONE)
			break;
	}

	update_state_flag(s);

	return s;
}

void update_state_flag(sudoku_state *s)
{
	if(s->status & STATUS_DONE)
		return;

	int i;
	for(i = 1; i <= n; ++i)
	{
		if(s->roles[i].num_options != 0)
			return;
	}

	s->status |= STATUS_DONE;
}

typedef struct choice
{
	sudoku_state *state;
	int role;
	int actor;

	struct choice *next;
} choice;

int num_states;

int min_score, max_score;

int best_score = 0x7FFFFFFF;
sudoku_state *best_state = NULL;

void score_state(sudoku_state *s)
{
	int score = n - s->assigned_roles + s->num_actors + s->num_super;
	if(score < best_score)
	{
		if(best_state != NULL)
		{
			unlink_state(best_state);
		}
		best_score = score;
		best_state = s;
		s->refcount++;
	}
}

choice **sorted_choices;

void gen_choices(sudoku_state *s)
{
	update_state_flag(s);
	int score = n - s->assigned_roles + s->num_actors + s->num_super;

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
	for(i = 1; i <= n; ++i)
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
	s->refcount += least_options;
	edge *e = s->roles[idx].options;

	choice *c_next = sorted_choices[score];

	choice *c;

	for(i = 0; i < least_options; ++i)
	{
		c = malloc(sizeof(choice));
		c->next = c_next;
		c_next = c;
		c->state = s;
		c->role = idx;

		c->actor = e->target;
		e = e->next;
	}
	sorted_choices[score] = c;
}

sudoku_state *run_choice(choice *c)
{
	// klona state

	sudoku_state *s;
	if(c->state->refcount == 1)
		s = c->state;
	else
	{
		s = clone_state(c->state);
		unlink_state(c->state);
	}

	// anropa assign_role
	assign_role(s, c->role, c->actor);

	// uppdatera state
	update_state_flag(s);
	return s;
}

void free_edge(edge *e)
{
	if(e->next != NULL)
		free_edge(e->next);
	free(e);
}

void free_state(sudoku_state *s)
{
	int i;
	for(i = 1; i <= n; ++i)
	{
		if(s->roles[i].num_options != 0)
			free_edge(s->roles[i].options);
	}
	free(s->roles+1);
	free(s->actors_rolecount+1);
	free(s);
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
	for(i = 1; i <= n; ++i)
	{
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
	for(i = 1; i <= k; ++i)
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
	for(i = 1; i <=n; ++i)
	{
		if(best_state->roles[i].num_options != 0)
			best_state->roles[i].actor = ++k;
	}
	print_output(best_state);
}

void assign_second(sudoku_state *s)
{
	if(s->status & STATUS_DIVA2_ASSIGNED)
	{
		update_state_flag(s);
		gen_choices(s);
		score_state(s);
		return;
	}

	int i;
	for(i = 1; i <= n; ++i)
	{
		edge *e;

		if(s->roles[i].num_options == 0)
			continue;

		for(e = s->roles[i].options; e != NULL; e = e->next)
		{
			if(e->target == 2)
			{
				sudoku_state *s_new = clone_state(s);
				assign_role(s_new, i, 2);
				update_state_flag(s_new);
				gen_choices(s_new);
				score_state(s_new);
				unlink_state(s_new);
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
	for(i = 1; i <= n; ++i)
	{
		edge *e;

		if(s->roles[i].num_options == 0)
			continue;

		for(e = s->roles[i].options; e != NULL; e = e->next)
		{
			if(e->target == 1)
			{
				sudoku_state *s_new = clone_state(s);
				assign_role(s_new, i, 1);
				assign_second(s_new);
				unlink_state(s_new);

				break;
			}
		}
	}
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

	sorted_choices = calloc(k + n, sizeof(choice *));

	min_score = n+k-1;
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

			unlink_state(c->state);
			free(c);

			while(sorted_choices[min_score] == NULL && min_score < max_score)
				min_score++;
		}

		min_score = n+k-1;
		max_score = 0;

		// kör alla val
		while(prev_choices != NULL)
		{
			choice *next = prev_choices->next;
			sudoku_state *s = run_choice(prev_choices);

			free(prev_choices);

			gen_choices(s);
			unlink_state(s);

			prev_choices = next;
		}
	}

	free(sorted_choices);

	print_best();
	unlink_state(s);
	//unlink_state(best_state);
	//print_output(s);
}

