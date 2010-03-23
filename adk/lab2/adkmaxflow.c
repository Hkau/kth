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

typedef struct flow
{
	int target;
	int value;
	struct flow *next;
} flow_t;

//graph types
typedef struct edge
{
	int target;
	int capacity;
	// int backpacity // stuff used by 'bro!

	struct edge *bro; // komplementkant (backflow)
	struct edge *next;

	flow_t *flow;
} edge_t;

typedef struct
{
	edge_t *first_edge;
	edge_t *last_edge;	// fast in-order assignment thing; 
						// sort on first read?

	flow_t *first_flow;
	flow_t *last_flow;
} vertex_t;

// graph metadata
int num_vertices;

int source_idx;
int drain_idx;

int num_edges;

// graphs

vertex_t *v;

edge_t *edges;
int edge_index = 0;

flow_t *flows;
int flow_index = 0;

static inline void spawn_edge(int from, int to, int capacity)
{
	printf("edge:: from: %d, to: %d, capacity: %d\n", from, to, capacity);

	edge_t *from_edge, *to_edge;
	if(v[from].first_edge != NULL)
	{
		for(from_edge = v[from].first_edge; from_edge != NULL; from_edge = from_edge->next)
			if(from_edge->target == to)
			{
				// kanten finns redan! (tidigare inläst backedge)
				from_edge->capacity = capacity;

				// TODO: Skapa riktig kant då! :)
				if(v[from].first_flow != NULL)
				{
					v[from].last_flow->next = &flows[flow_index];
					v[from].last_flow = &flows[flow_index];
				}
				else
				{
					v[from].first_flow = v[from].last_flow = &flows[flow_index];
				}
				flows[flow_index].target = to;
				from_edge->flow = &flows[flow_index++];
				return;
			}

		// finns ingen kant mellan from eller to, måste fixas!
		v[from].last_edge->next = from_edge = &edges[edge_index++];
		v[from].last_edge = from_edge;
	}
	// finns ingen kant, måste åtgärdas! :D
	else
	{
		v[from].last_edge = v[from].first_edge = from_edge = &edges[edge_index++];
	}

	from_edge->capacity = capacity;
	// TODO: Skapa riktig from-kant
	if(v[from].first_flow != NULL)
	{
		v[from].last_flow->next = &flows[flow_index];
		v[from].last_flow = &flows[flow_index];
	}
	else
	{
		v[from].first_flow = v[from].last_flow = &flows[flow_index];
	}

	flows[flow_index].target = to;
	from_edge->flow = &flows[flow_index++];

	// skapa samtidigt back-edge
	if(v[to].first_edge != NULL)
	{
		v[to].last_edge->next = to_edge = &edges[edge_index++];
		v[to].last_edge = to_edge;
	}
	else
	{
		v[to].last_edge = v[to].first_edge = to_edge = &edges[edge_index++];
	}

	from_edge->bro = to_edge;
	to_edge->bro = from_edge;

	from_edge->target = to;
	to_edge->target = from;
}

void read_graph()
{
	num_vertices = readint();
	source_idx = readint();
	drain_idx = readint();

	num_edges = readint();

	printf("num verts: %d, src: %d, drain: %d, num_edges: %d\n",
			num_vertices, source_idx, drain_idx, num_edges);

	// alloc vertices!!! :D
	v = calloc(num_vertices+1, sizeof(vertex_t));
	edges = calloc(2*num_edges, sizeof(edge_t)); // ska räcka :)
	flows = calloc(num_edges, sizeof(flow_t));

	int i;
	for(i = 0; i < num_edges; ++i)
	{
		int from = readint();
		int to = readint();
		int capacity = readint();

		spawn_edge(from, to, capacity);
	}
}

void print_flow()
{
	int i;

	for(i = 1; i <= num_vertices; ++i)
	{
		edge_t *edge;
		for(edge = v[i].first_edge; edge != NULL; edge = edge->next)
			printf("%d %d %d\n", i, edge->target, edge->capacity);
	}
}

void print_graph()
{
	int i;

	for(i = 1; i <= num_vertices; ++i)
	{
		flow_t *flow;

		for(flow = v[i].first_flow; flow != NULL; flow = flow->next)
			printf("%d %d %d\n", i, flow->target, flow->value);
	}
}

int main()
{
	read_graph();

puts("");
	print_flow();
puts("");
	print_graph();

	return 0;
}

