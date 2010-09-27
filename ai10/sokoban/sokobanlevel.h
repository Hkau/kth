#ifndef SOKOBANLEVEL_H
#define SOKOBANLEVEL_H

#include <deque>
#include <vector>
#include <string>

#include "position.h"

/*
Level element	   Character   ASCII Code
Wall				'#'			0x23
Player				'@'			0x40
Player on goal  	'+'			0x2b
Box				 '$'			0x24
Box on goal	 	'*'			0x2a
Goal square		 '.'		 0x2e
Floor			   ' '			0x20
*/
static const char WALL		= '#';
static const char PLAYER	  = '@';
static const char PLAYER_GOAL = '+';
static const char BOX		 = '$';
static const char BOX_GOAL	= '*';
static const char GOAL		= '.';
static const char FLOOR	   = ' ';
static const char MOVE_UP = 'U';
static const char MOVE_DOWN = 'D';
static const char MOVE_LEFT = 'L';
static const char MOVE_RIGHT = 'R';
static const char MOVE_NONE = ' ';

struct Node
{
	Node(Position p, unsigned int cost) : p(p), c(cost) {}
	Position p;
	unsigned int c;
};

class SokobanLevel
{
private:
	int width;
	int height;
	int *poscost;
	std::vector<Position> boxes;
	std::vector<Position> goals;
	Position player;
	std::vector<std::string> level;

	void BuildPositionCosts()
	{
		unsigned int size = width*height;
		delete poscost;
		poscost = new int[size];
		for (unsigned int i = 0; i < size; ++i)
			poscost[i] = -1;
		// BFS from goals outwards with only valid backward moves

		std::deque<Position> queue;
		for (unsigned int i = 0; i < goals.size(); ++i)
		{
			queue.push_back(goals[i]);
			PosCost(goals[i]) = 0;
		}

		while (queue.empty() == false)
		{
			Position p = queue.front();
			queue.pop_front();
			int c = PosCost(p);

			// expandera noden
			// kolla vilka positioner som kan leda hit och inte ännu är markerade
			// markera kostnaden för dem och lägg in dem i kön
			Position left(p.x-1, p.y);
			Position right(p.x+1, p.y);
			Position up(p.x, p.y-1);
			Position down(p.x, p.y+1);

			if (ValidMove(left, p) && PosCost(left) == -1)
			{
				PosCost(left) = c+1;
				queue.push_back(left);
			}
			if (ValidMove(right, p) && PosCost(right) == -1)
			{
				PosCost(right) = c+1;
				queue.push_back(right);
			}
			if (ValidMove(up, p) && PosCost(up) == -1)
			{
				PosCost(up) = c+1;
				queue.push_back(up);
			}
			if (ValidMove(down, p) && PosCost(down) == -1)
			{
				PosCost(down) = c+1;
				queue.push_back(down);
			}
		}
	}

public:
	SokobanLevel(const char *str) :
		width(0), height(0), poscost(0)
	{
		level.push_back("");
		int x = 0;
		int y = 0;
		for (; *str != 0; ++str)
		{
			if (*str == '\n')
			{
				x = 0;
				++y;
				level.push_back("");
				continue;
			}
			if (*str == PLAYER || *str == PLAYER_GOAL)
			{
				player.x = x;
				player.y = y;
			}

			if (*str == BOX || *str == BOX_GOAL)
				boxes.push_back(Position(x,y));

			if (*str == GOAL || *str == PLAYER_GOAL || *str == BOX_GOAL)
				goals.push_back(Position(x,y));


			level[y].push_back(*str);
			++x;
			if (*str == WALL && x > width)
					width = x;
		}
		if (str[-1] == '\n')
			height = y;
		else
			height = y+1;

		BuildPositionCosts();
	}

	bool ValidMove(const Position &from, const Position &to)
	{
		if (to.x >= width || to.y >= height || to.x < 0 || to.y < 0)
			return false;
		if (from.x >= width || from.y >= height || from.x < 0 || from.y < 0)
			return false;

		// Is it a wall on the way?
		if (level[to.y][to.x] == WALL)
			return false;
		if (level[from.y][from.x] == WALL)
			return false;

		// Is it a wall where the player would stand if pushing this box?
		Position p = from - (to - from);

		if (p.x >= width || p.y >= height || p.x < 0 || p.y < 0)
			return false;

		if (level[p.y][p.x] == WALL)
			return false;

		return true;
	}

	const int & PosCost(int x, int y) const
	{
		return poscost[y*width + x];
	}

	int & PosCost(int x, int y)
	{
		return poscost[y*width + x];
	}

	const int & PosCost(const Position p) const
	{
		return poscost[p.y*width + p.x];
	}

	int & PosCost(const Position p)
	{
		return poscost[p.y*width + p.x];
	}

	unsigned int Num_boxes() const
	{
		return boxes.size();
	}

	Position & getBox(int i)
	{
		return boxes[i];
	}

	const Position & getBox(int i) const
	{
		return boxes[i];
	}

	Position & getPlayer()
	{
		return player;
	}

	const Position & getPlayer() const
	{
		return player;
	}

	void print() const
	{
		for (int i = 0; i < height; ++i)
			printf("%s\n", level[i].c_str());

		for (int x = 0; x < width; ++x)
			printf(" __");
		putchar('\n');
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				int c = PosCost(x,y);
				if(c == 0)
					printf("|<>");
				else if (c < 0)
					printf("|__");
				else
					printf("|%2d", c);
			}
			printf("|\n");
		}

	}
	friend class SokobanState;

	int GetWidth() const
	{
		return width;
	}
	int GetHeight() const
	{
		return height;
	}

	char GetMapPos(int x, int y) const
	{
		return level[y][x];
	}
	char GetMapPos(const Position &p) const
	{
		return GetMapPos(p.x, p.y);
	}
};

#endif // SOKOBANLEVEL_H
