#include <stdio.h>
#include <stdlib.h>

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

typedef struct edge
{
	int target;

	struct edge *next;
} edge;

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
		exit(37);
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

	int assigned_roles;
	state_role *roles;

} sudoku_state;

sudoku_state *clone_state(sudoku_state *s)
{
	sudoku_state *new_state = calloc(1,sizeof(sudoku_state));

	*new_state = *s;

	new_state->roles = (state_role *) malloc(n * sizeof(state_role)) - 1;

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

void remove_actor(sudoku_state *s, int role, int actor)
{
	// Iterera över alla möjliga skådespelare för rollen och stryk ev. förekomst av actor.
	// Om det bara finns ett alternativ kvar, tillsätt rollen rekursivt.
	// Om rollen saknar alternativ, skapa en superskådis, eller panika.

	if(s->roles[role].num_options == 0)
	{
		if(s->roles[role].actor == actor)
			s->status = STATUS_INVALID;
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

	s->assigned_roles ++;
	s->roles[role].actor = actor;
	s->roles[role].num_options = 0;

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

sudoku_state *global_state;

sudoku_state *first_state()
{
	sudoku_state *s = calloc(1, sizeof(sudoku_state));

	s->roles = (state_role *)malloc(n*sizeof(state_role)) - 1;

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
	int role;
	int actor;
	int refcount;

	struct choice *next, *parent;
} choice;

int num_states;

int min_assigned, max_assigned;

choice **sorted_choices;

void gen_choices(choice *parent, sudoku_state *s)
{
	int assign = s->assigned_roles;

	if(max_assigned < assign)
		max_assigned = assign;
	
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

	// lägg in alla alternativ i den sorterade listan av choices
	edge *e = s->roles[idx].options;
	if(parent != NULL)
		parent->refcount = least_options;

	choice *c_next = sorted_choices[assign];

	choice *c;

	for(i = 0; i < least_options; ++i)
	{
		c = malloc(sizeof(choice));
		c->next = c_next;
		c_next = c;
		c->parent = parent;
		c->role = idx;
		c->refcount = 0;

		c->actor = e->target;
		e = e->next;
	}
	sorted_choices[assign] = c;
}

void assign_choices(choice *c, sudoku_state *s)
{
	if(c == NULL)
		return;
	assign_choices(c->parent, s);
	assign_role(s, c->role, c->actor);
}

void free_choice(choice *c)
{
	if(c->parent != NULL)
	{
		c->parent->refcount --;
		if(c->parent->refcount == 0)
			free_choice(c->parent);
	}
	free(c);
}

sudoku_state *run_choice(choice *c)
{
	// klona state

	sudoku_state *s = clone_state(global_state);

	assign_choices(c, s);

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
		if(s->roles[i].options != NULL)
			;//free_edge(s->roles[i].options);
	}
	free(s->roles+1);
	free(s);
}

void print_output(sudoku_state *s)
{
	// Utdataformat:
	// Rad ett: antal skådespelare som fått roller

	int *num_assigned = (int *)calloc(k, sizeof(int))-1;
	edge **assigned = (edge **)calloc(k, sizeof(edge *)) - 1;
	int assigned_roles = 0;

	int i;
	for(i = 1; i <= n; ++i)
	{
		int actor = s->roles[i].actor;

		edge *e = malloc(sizeof(edge));
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

void sudoku_casting()
{
	// Skapa ett state som beskriver ett graftillstånd att börja jobba på.
	// Alla entydiga lösningssteg utförs.
	global_state = first_state();

	// kolla ifall tillståndet representerar en korrekt lösning
	update_state_flag(global_state);

	// skriv ut om den redan är entydigt klar
	if(global_state->status & STATUS_DONE)
	{
		print_output(global_state);
		return;
	}

	// om vi kommer till ett val så skapa alla val som behövs men så få som möjligt
	// stoppa in valen i dinmamma
	// ta ut ett val med flest tilldelade roller och bearbeta det
	// loopa

	sorted_choices = calloc(n, sizeof(choice *));

	min_assigned = max_assigned = global_state->assigned_roles;

	gen_choices(NULL, global_state);

	sudoku_state *s;

	while(1)
	{
		choice *c = sorted_choices[max_assigned];
		sorted_choices[max_assigned] = c->next;

		while(sorted_choices[max_assigned] == NULL && max_assigned > min_assigned)
			max_assigned--;

		s = run_choice(c);

		if((s->status & STATUS_SUCCESS) == STATUS_SUCCESS)
			break;

		if((s->status & STATUS_DONE) == 0)
			gen_choices(c, s);
		else
			free_choice(c);

		free_state(s);
	}

	print_output(s);
}

