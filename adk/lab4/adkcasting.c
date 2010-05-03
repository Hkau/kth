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

	print_graph();

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
	STATUS_PENDING = 0,
	STATUS_INVALID = 13,
	STATUS_DONE = 42
};

typedef struct
{
	int total_options;
	enum status_flag status;
	state_role *roles;
} sudoku_state;

sudoku_state *clone_state(sudoku_state *s)
{
	sudoku_state *new_state = malloc(sizeof(sudoku_state));
	new_state->status = STATUS_PENDING;

	new_state->total_options = s->total_options;
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

	if(s->status == STATUS_INVALID)
		puts("gick inte bra :(");
	else if(s->status == STATUS_DONE)
		puts("fäärdig <3");
	else if(s->status == STATUS_PENDING)
		puts("working on it, boss!");
}

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
		s->roles[role].options = opts->next;

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
			opts->next = opts->next->next;

			// Om rollen är entydig
			if(s->roles[role].num_options == 1)
				// Tillsätt rollen
				assign_role(s, role, s->roles[role].options->target);
			return;
		}
	}
}

void assign_role(sudoku_state *s, int role, int actor)
{
	s->roles[role].actor = actor;
	s->roles[role].num_options = 0;

	// Stryk alla alternativ i konlikt med detta val och alla val som tvingas av detta
	// Iterera över varje kant, e, i statiska scengrafen som symboliserar att två roller deltar i
	// samma scen och stryk alla förekomster av skådespelaren actor för att undvika konflikter.

	if(actor <= 2)
	{
		// jobbiga divor :(
		edge *e;
		int other_actor = 3 - actor; // (actor == 1) ? 2 : 1;
		for(e = roles[role].first_edge; e != NULL; e = e->next)
		{
			remove_actor(s, e->target, other_actor);
			if(s->status != STATUS_PENDING)
				return;
		}

		remove_actor(s, role, other_actor);
		if(s->status != STATUS_PENDING)
			return;
	}

	edge *e;
	for(e = roles[role].first_edge; e != NULL; e = e->next)
	{
		remove_actor(s, e->target, actor);
		if(s->status != STATUS_PENDING)
			return;
	}
}

sudoku_state *first_state()
{
	sudoku_state *s = malloc(sizeof(sudoku_state));
	s->status = STATUS_PENDING;

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
		if(s->status != STATUS_PENDING)
			break;
	}

	return s;
}

void update_state_flag(sudoku_state *s)
{
	if(s->status != STATUS_PENDING)
		return;

	int i;
	for(i = 1; i <= n; ++i)
	{
		if(s->roles[i].num_options != 0)
			return;
	}

	s->status = STATUS_DONE;
}

void sudoku_casting()
{
	// Skapa ett state som beskriver ett graftillstånd att börja jobba på.
	// Alla entydiga lösningssteg utförs.
	sudoku_state *s = first_state();

	// kolla ifall tillståndet representerar en korrekt lösning
	update_state_flag(s);


	print_state(s);

	//while ( s = pickOption != NULL) ish 
}
