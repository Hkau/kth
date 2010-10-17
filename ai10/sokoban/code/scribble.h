#ifndef SCRIBBLE_H
#define SCRIBBLE_H

#include <deque>
#include <vector>

class SokobanLevel;
#include "position.h"

static const unsigned int WALL_COLOR = (unsigned int)-1; // infinite

class Scribble
{
	const int width;
	const int height;
	const unsigned size;
	int * const poscost; // Contains position costs and goal reachability

	// permanent storage for walls and temporary storage for boxes and player reachability
	unsigned int * const scribble;
	unsigned int scribble_color;

	std::vector<Position> goals;

	bool ValidMove(const Position &from, const Position &to) const;

	unsigned int & ColorAt(const Position &p) const
	{   return scribble[p.y * width + p.x];	}

	unsigned int & ColorAt(int x, int y) const
	{   return scribble[y * width + x];	}

public:
	Scribble(const SokobanLevel &level);
	~Scribble();

	unsigned int ClearScribble();
	void BuildPositionCosts();

	void Print() const;

	int GetWidth() const
	{	return width;	}

	int GetHeight() const
	{	return height;	}

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

	int & PosCost(int x, int y)
	{   return poscost[y*width + x];	}

	int & PosCost(const Position p)
	{   return poscost[p.y*width + p.x];	}

	const int & PosCost(int x, int y) const
	{   return poscost[y*width + x];	}

	const int & PosCost(const Position p) const
	{   return poscost[p.y*width + p.x];	}

	int & GoalsReachable(const Position p)
	{	return poscost[width*height + p.y*width + p.x];	}

	const int & GoalsReachable(const Position p) const
	{	return poscost[width*height + p.y*width + p.x];	}

	bool ValidPosition(const Position &p) const
	{   return (p.x>=0 && p.x<width && p.y>=0 && p.y<height);	}

	bool ValidPosition(int x, int y) const
	{   return (x>=0 && x<width && y>=0 && y<height);	}

	bool IsWall(const Position &p) const
	{   return ColorAt(p) == WALL_COLOR;	}

	bool IsBox(const Position &p) const
	{   return ColorAt(p) == scribble_color;	}

	bool Blocked(const Position &p) const
	{   return ColorAt(p) >= scribble_color;	}

	void MarkBox(const Position &p) const
	{
		if (ColorAt(p) < scribble_color)
			ColorAt(p) = scribble_color;
	}

	void MarkWall(const Position &p) const
	{   ColorAt(p) = WALL_COLOR;	}

	void MarkReachable(const Position &p) const
	{   ColorAt(p) = scribble_color-1;	}

	bool IsUntouchedFloor(const Position &p) const
	{   return ColorAt(p)  < scribble_color-1;	}

	bool IsReachable(const Position &p) const
	{   return ColorAt(p)  == scribble_color-1;	}
};

#endif // SCRIBBLE_H
