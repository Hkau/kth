#ifndef POSITION_H
#define POSITION_H

struct Position
{
	int x,y;

	Position() : x(0), y(0)
	{

	}

	Position(int x, int y) : x(x), y(y)
	{

	}

	Position operator-(const Position &b) const
	{
		return Position(x - b.x, y - b.y);
	}

	Position operator+(const Position &b) const
	{
		return Position(x + b.x, y + b.y);
	}

	bool operator==(const Position &b) const
	{
		return x == b.x && y == b.y;
	}

	bool operator!=(const Position &b) const
	{
		return x != b.x || y != b.y;
	}
	bool operator <(const Position &b) const
	{
		if(this->y < b.y)
			return true;

		if(this->y > b.y)
			return false;

		return this->x < b.x;
	}
};

#endif // POSITION_H
