#ifndef SOKOBANSTATE_H
#define SOKOBANSTATE_H

#include <assert.h>
#include <string.h>

#include "lookup3.h"
#include "position.h"
#include "sokobanlevel.h"
#include "queue.h"

#include <stack>

//static int fake_hash = 0;

class SokobanState
{
private:
	// En referens till banan för att kunna kolla vilka drag som är möjliga
	const SokobanLevel &level;
	// Lådornas positioner
	int num_boxes;

	Position player;
	Position *boxes;

	int last_box;
	char last_move;

	// Kostnad
	int cost;

	// Referensräkning
	const SokobanState *parent;
	int refs; // Hur många barn som pekar hit

public:
	SokobanState(const SokobanLevel &level) :
		level(level),
		num_boxes(level.Num_boxes()),
		boxes(new Position[num_boxes]),
		player(level.getPlayer()),
		last_box(-1),
		last_move(MOVE_NONE),
		parent(NULL)
	{
		for(int i = 0; i < num_boxes; ++i)
			boxes[i] = level.getBox(i);
		CalcCost();
	}
	// create new state with one new move applied
	// note: (assumed to be valid, check before)
	SokobanState(const SokobanState &state, int box, char move):
		level(state.level),
		num_boxes(state.num_boxes),
		boxes(new Position[num_boxes]),
		player(state.player),
		parent(&state),
		last_box(box),
		last_move(move)
	{
//		printf("%d, %c\n", box, move);
		for(int i = 0; i < num_boxes; ++i)
		{
			boxes[i] = state.boxes[i];
		}


		// printf("move (%c) w/ box:(%d, %d)\n", move, boxes[box].x, boxes[box].y);

		player = boxes[box];
		switch(move)
		{
			case MOVE_UP:
				boxes[box].y -= 1;
				for(int idx = box - 1; idx >= 0; --idx)
				{
					if(boxes[idx] < boxes[box])
						break; // done, all earlier in order
					Position p = boxes[idx];
					boxes[idx] = boxes[box];
					boxes[box] = p;
					box --;
				}
				break;
			case MOVE_DOWN:
				boxes[box].y += 1;
				for(int idx = box+1; idx < num_boxes; ++idx)
				{
					if(boxes[box] < boxes[idx])
						break; // done, all earlier in order
					Position p = boxes[idx];
					boxes[idx] = boxes[box];
					boxes[box] = p;
					box ++;
				}
				break;
			case MOVE_LEFT:
				// will be sorted, or collide, but move is assumed to be valid
				boxes[box].x -= 1;
				break;
			case MOVE_RIGHT:
				// will be sorted, or collide, but move is assumed to be valid
				boxes[box].x += 1;
				break;
			default:
				assert(0);
		}
		CalcCost();
		// finally update player pos
	}

	void CalcCost()
	{
		cost = 0;
		for(int i = 0; i < num_boxes; ++i)
		{
			int pos_cost = level.PosCost(boxes[i]);
//			printf("pos_cost: (%d, %d) = %d\n", boxes[i].x, boxes[i].y, pos_cost);
			assert(pos_cost >= 0);
			cost += pos_cost;
		}
	}

	~SokobanState()
	{
		delete [] boxes;
		boxes = NULL;
	}

	unsigned int Cost() const
	{
		return cost;
	}

	std::string Moves() const
	{
		if (parent == NULL)
		{
			return std::string("");
		}

		int width = level.GetWidth(), height = level.GetHeight();
		char *reachable = new char[width*height];
		memset(reachable, FLOOR, sizeof(char)*width*height);

		for(int y = 0; y < height; ++y)
		{
			for(int x = 0; x < width; ++x)
			{
				if(level.GetMapPos(x, y) == WALL)
					reachable[y*width + x] = WALL;
			}
		}

		for(int i = 0; i < num_boxes; ++i)
		{
			reachable[parent->boxes[i].y * width + parent->boxes[i].x] = BOX;
		}

		// bfs time, go fill that shit.

		std::deque<Position> queue;
		queue.push_back(parent->player);

		reachable[parent->player.y * width + parent->player.x] = PLAYER;

		static const char move_name[] = {
			MOVE_UP,
			MOVE_DOWN,
			MOVE_LEFT,
			MOVE_RIGHT};

		Position goal = player;// parent->boxes[last_box];

		switch(last_move)
		{
			case(MOVE_UP):
				goal.y ++;
				break;
			case(MOVE_DOWN):
				goal.y --;
				break;
			case MOVE_LEFT:
				goal.x ++;
				break;
			case MOVE_RIGHT:
				goal.x --;
				break;
			default:
				assert(0);
		}

		while (queue.empty() == false)
		{
			Position p = queue.front();
			queue.pop_front();

			Position moves[] = {
				Position (p.x, p.y-1),  // up
				Position (p.x, p.y+1),  // down
				Position (p.x-1, p.y),  // left
				Position (p.x+1, p.y)}; // right

			int i;
			for(i = 0; i < 4; ++i)
			{
				if(reachable[moves[i].y * width + moves[i].x] == FLOOR)
				{
					reachable[moves[i].y * width + moves[i].x] = move_name[i];
					if(moves[i] == goal)
						break;
					queue.push_back(moves[i]);
				}
			}
			if(moves[i] == goal)
				break;
		}

		assert(reachable[goal.y*width + goal.x] != FLOOR);

		Position p = goal;

		std::stack<char> moves;
		while(p != parent->player)
		{
			char move = reachable[p.y * width + p.x];
			moves.push(move);

			switch(move)
			{
				// inverse moves
				case MOVE_UP:
					p.y ++;
					break;
				case MOVE_DOWN:
					p.y --;
					break;
				case MOVE_LEFT:
					p.x++;
					break;
				case MOVE_RIGHT:
					p.x--;
					break;
				default:
					putchar(move);putchar('\n');
					assert(0);
			}
		}

		std::string movestr;
		while(!moves.empty())
		{
			char c = moves.top(); moves.pop();
			movestr += c;
		}
		movestr += last_move;

		return parent->Moves() + movestr;
	}

	friend bool Equal(const SokobanState *a, const SokobanState *b)
	{
		return *a == *b;
	}

	friend bool operator==(const SokobanState &a, const SokobanState &b)
	{
		if (a.cost != b.cost)
			return false;
		if (a.num_boxes != b.num_boxes)
			return false;

		for (int i = 0; i < b.num_boxes; ++i)
			if (a.boxes[i] != b.boxes[i])
				return false;

		// TODO: jämför spelarens position så att två olika
		// men likvärdiga positioner räknas som ekvivalenta
		// replace nåbara ist. för player pos
		if (a.player != b.player)
			return false;

		// Om inget skiljer så är de ekvivalenta
		return true;
	}

	friend uint32_t Hash(const SokobanState *s)
	{
//		return fake_hash++;
		return hashword( (uint32_t*) s->boxes, (s->num_boxes<<1), hashword((uint32_t*)&s->player, 2, 0));
	}

	bool SpawnChildren(Queue<SokobanState *> &q)
	{
		int width = level.GetWidth(), height = level.GetHeight();
		char *reachable = new char[width*height];
		memset(reachable, FLOOR, sizeof(char)*width*height);

		for(int y = 0; y < height; ++y)
		{
			for(int x = 0; x < width; ++x)
			{
				if(level.GetMapPos(x, y) == WALL)
					reachable[y*width + x] = WALL;
			}
		}

		for(int i = 0; i < num_boxes; ++i)
		{
			reachable[boxes[i].y * width + boxes[i].x] = BOX;
		}

		// bfs time, go fill that shit.

		std::deque<Position> queue;
		queue.push_back(player);

		reachable[player.y * width + player.x] = PLAYER;

		while (queue.empty() == false)
		{
			Position p = queue.front();
			queue.pop_front();

			Position moves[] = {Position (p.x-1, p.y), Position (p.x+1, p.y), Position (p.x, p.y-1), Position (p.x, p.y+1)};

			for(int i = 0; i < 4; ++i)
			{
				if(reachable[moves[i].y * width + moves[i].x] == FLOOR)
				{
					reachable[moves[i].y * width + moves[i].x] = PLAYER;
					queue.push_back(moves[i]);
				}
			}
		}

/*		for(int y = 0; y < height; ++y)
		{
			for(int x = 0; x < width; ++x)
			{
				char c = reachable[y*width + x];
				if(c == PLAYER)
					c = '.';
				putchar(c);
			}
			putchar('\n');
		}*/

		static const char move_name[] = {
				MOVE_UP,
				MOVE_DOWN,
				MOVE_LEFT,
				MOVE_RIGHT};

		//printf("moves: ");
		for(int i = 0; i < num_boxes; ++i)
		{
			// gå igenom alla positioner kring lådan

			Position &p = boxes[i];


			char mappos[] = {
				reachable[(p.y+1) * width + p.x], // move from down (move box up)
				reachable[(p.y-1) * width + p.x], // move from up (move box down)
				reachable[p.y * width + p.x+1],   // move from right (move box left)
				reachable[p.y * width + p.x-1]};  // move from left (move box right)

			char movepos[] = {
				reachable[(p.y-1) * width + p.x], // move box up
				reachable[(p.y+1) * width + p.x], // move box down
				reachable[p.y * width + p.x-1],   // move box left
				reachable[p.y * width + p.x+1]};  // move box right

			int costs[] = {
				level.PosCost(p.x, p.y-1),  // move box up
				level.PosCost(p.x, p.y+1),  // move box down
				level.PosCost(p.x-1, p.y),  // move box left
				level.PosCost(p.x+1, p.y)}; // move box right

			for(int j = 0; j < 4; ++j)
			{
				if(mappos[j] != PLAYER || movepos[j] == BOX || costs[j] < 0)
					continue;

				// valid move, lets do it!
				//printf("(%d: %c) ", i, move_name[j]);
				SokobanState *s = new SokobanState(*this, i, move_name[j]);
				q.Push(s, s->Cost());
			}

			// cost >= 0? OK
			// spelare på rätt sida? OK!

			// annars continue;
		}
		//putchar('\n');
	}

};

#endif // SOKOBANSTATE_H
