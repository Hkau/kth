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

//graph types
typedef struct edge
{
	int target;
	int capacity;

	// additional capacity (external flow, needs to be updated)
	int flow; // stuff used by 'bro!

	struct edge *bro; // komplementkant (backflow)
	struct edge *next;
} edge_t;

typedef struct
{
	int last_visit;

	edge_t *first_edge;
	edge_t *last_edge;	// fast in-order assignment thing; 
				// sort on first read?
} vertex_t;

// graph metadata
int num_vertices;

int source_idx;
int drain_idx;

int num_edges;

// graphs

vertex_t v[4001];
//vertex_t *v;

edge_t edges[20000];
//edge_t *edges;

int edge_index = 0;

typedef struct bfsnode
{
	int vertex;

	edge_t *edge;
	struct bfsnode *parent;
} bfsnode_t;

bfsnode_t bfsnodes[4001];

int visits = 0;

bfsnode_t *find_path()
{
	bfsnode_t *bfs_lower = bfsnodes;
	bfsnode_t *bfs_upper = bfsnodes+1;
	++visits;

	bfs_lower->vertex = source_idx;

	while(bfs_lower < bfs_upper)
	{
		int idx = bfs_lower->vertex;
		edge_t *e;
		for(e = v[idx].first_edge; e != NULL; e = e->next)
		{
			int v_idx = e->target;

			if(v[v_idx].last_visit == visits || e->capacity < 1)
				continue;

			v[v_idx].last_visit = visits;

			bfs_upper->vertex = v_idx;
			bfs_upper->edge = e;
			bfs_upper->parent = bfs_lower;

			if(v_idx == drain_idx)
				return bfs_upper;
			bfs_upper++;
		}

		bfs_lower++;
	}

	return NULL;
}

#define min(x, y) ((x <= y)? x : y)

int path_flow(bfsnode_t *path)
{
	// HISSATSU
	int flow = 0x7FFFFFFF;
	//int flow = path->edge->capacity + path->edge->rest_flow;

	while(path->edge != NULL)
	{
		if(path->edge->capacity < flow)
			flow = path->edge->capacity;
		path = path->parent;
	}
	return flow;
}

static inline void update_flow(bfsnode_t *path, int flow)
{
	edge_t *edge;

	while((edge = path->edge) != NULL)
	{
		edge->capacity -= flow;
		edge->bro->capacity += flow;
		edge->flow += flow;
		edge->bro->flow -= flow;

		path = path->parent;
	}
}

void perform_flow()
{
	bfsnode_t *flowpath;

	while((flowpath = find_path()) != NULL)
	{
		int flow = path_flow(flowpath);

		update_flow(flowpath, flow);
	}
}

static inline void spawn_edge(int from, int to, int capacity)
{
	//printf("edge:: from: %d, to: %d, capacity: %d\n", from, to, capacity);

	edge_t *from_edge, *to_edge;

	if(v[from].first_edge != NULL)
	{
		for(from_edge = v[from].first_edge; from_edge != NULL; from_edge = from_edge->next)
			if(from_edge->target == to)
			{
				// kanten finns redan! (tidigare inläst backedge)
				from_edge->capacity = capacity;
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

	//printf("num verts: %d, src: %d, drain: %d, num_edges: %d\n",
	//		num_vertices, source_idx, drain_idx, num_edges);

	// alloc vertices!!! :D
	//v = calloc(num_vertices+1, sizeof(vertex_t));
	//edges = calloc(2*num_edges, sizeof(edge_t)); // ska räcka :)

	//bfsnodes = malloc(num_vertices*sizeof(bfsnode_t));
	bfsnodes->edge = NULL;
	bfsnodes->parent = NULL;

	int i;
	for(i = 0; i < num_edges; ++i)
	{
		int from = readint();
		int to = readint();
		int capacity = readint();

		spawn_edge(from, to, capacity);
	}
}

void print_graph()
{
	int i = 0;

	edge_t *edge;
	for(edge = v[source_idx].first_edge; edge != NULL; edge = edge->next)
	{
		if(edge->flow >0)
			i += edge->flow;

		//i+=(flow->value > 0)*flow->value;
	}

	// total flow! :)
	printf("%d\n%d %d %d\n", num_vertices, source_idx, drain_idx, i);


	int flows = 0;
	for(i = 1; i <= num_vertices; ++i)
	{
		for(edge = v[i].first_edge; edge != NULL; edge = edge->next)
			if(edge->flow > 0)
				++flows;
			//flows += (edge->flow > 0);
	}

	printf("%d\n", flows);

	for(i = 1; i <= num_vertices; ++i)
	{
		edge_t *edge;

		for(edge = v[i].first_edge; edge != NULL; edge = edge->next)
			if(edge->flow > 0)
				printf("%d %d %d\n", i, edge->target, edge->flow);
	}
}

int main()
{
	read_graph();

	perform_flow();

	print_graph();

	return 0;
}

