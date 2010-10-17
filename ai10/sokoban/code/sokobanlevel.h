#ifndef SOKOBANLEVEL_H
#define SOKOBANLEVEL_H

#include <deque>
#include <vector>
#include <string>
#include <map>

#include "position.h"
#include "scribble.h"

/*
Level element	   Character   ASCII Code
Wall				'#'			0x23
Player				'@'			0x40
Player on goal  	'+'			0x2b
Box					'$'			0x24
Box on goal			'*'			0x2a
Goal square			'.'			0x2e
Floor				' '			0x20
*/
static const char WALL			= '#';
static const char PLAYER		= '@';
static const char PLAYER_GOAL	= '+';
static const char BOX			= '$';
static const char BOX_GOAL		= '*';
static const char GOAL			= '.';
static const char FLOOR			= ' ';
static const char LOCKED_BOX	= 'x';

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

	mutable std::map<int, Scribble*> scribbles;

	std::vector<Position> boxes;
	std::vector<Position> goals;
	Position player;
	std::vector<std::string> level;

	// assignment not allowed
	void operator=(const SokobanLevel &);
	SokobanLevel(const SokobanLevel &sl);

public:
	SokobanLevel(const char *str) :
		width(0),
		height(0)
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
		if (str[-2] == '\n')
			height = y-1;
		else if (str[-1] == '\n')
			height = y;
		else
			height = y+1;
	}

	~SokobanLevel()
	{
	}

	void Print() const
	{
		for (int i = 0; i < height; ++i)
			printf("%s\n", level[i].c_str());

		/*
		for (int x = 0; x < width; ++x)
			printf(" __");
		putchar('\n');
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				int c = scribble->PosCost(x,y);
				if(c == 0)
					printf("|<>");
				else if (c > 0)
					printf("|%2d", c);
				else if (level[y][x] == '#')
					printf("|##");
				else
					printf("|__");
			}
			printf("|\n");
		}
		*/
	}

public:

	Scribble * GetScribble(int free_goals) const
	{
		std::map<int, Scribble*>::iterator it = scribbles.find(free_goals);
		if (it != scribbles.end())
			return it->second;

		//it = scribbles.insert(std::pair<int, Scribble*>(free_goals, new Scribble(*this)));
		Scribble *s = new Scribble(*this);
		scribbles[free_goals] = s;

		for (unsigned i = 0; i < goals.size(); ++i)
		{
			// Är målet blockerad?
			if ( ((1<<i) & free_goals) == 0 )
				s->MarkWall(goals[i]);
		}
		s->BuildPositionCosts();
		//printf("************* SCRIBBLE %d CREATED *************\n", scribbles.size());
		return s;
	}

	unsigned int NumScribbles() const
	{	return scribbles.size();	}

	unsigned int NumBoxes() const
	{	return boxes.size();	}

	const Position & GetBox(int i) const
	{	return boxes[i];	}

	const Position & GetPlayer() const
	{	return player;	}

	const Position & GetGoal(int i) const
	{	return goals[i];	}

	const std::vector<Position> & GetGoals() const
	{	return goals;	}

	int GetGoalAt(const Position &p) const
	{
		for (size_t i = 0; i < goals.size(); ++i)
		{
			if (goals[i] == p)
				return i;
		}
		return -1;
	}

	int GetWidth() const
	{	return width;	}

	int GetHeight() const
	{	return height;	}

	char GetMapPos(int x, int y) const
	{	return level[y][x];	}

	char GetMapPos(const Position &p) const
	{	return level[p.y][p.x];	}

	bool ValidPosition(const Position &p) const
	{   return (p.x>=0 && p.x<width && p.y>=0 && p.y<height);	}

	bool ValidPosition(int x, int y) const
	{   return (x>=0 && x<width && y>=0 && y<height);	}

};

#endif // SOKOBANLEVEL_H
