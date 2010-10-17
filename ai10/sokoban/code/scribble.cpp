#include <deque>
#include <vector>

#include "scribble.h"
#include "sokobanlevel.h"

Scribble::Scribble(const SokobanLevel &level) :
        width(level.GetWidth()),
        height(level.GetHeight()),
        size(width*height),
        poscost(new int[size<<1]),
        scribble(new unsigned int[size]),
        scribble_color(0),
        goals(level.GetGoals())
{
    int i = 0;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (level.GetMapPos(x, y) == WALL)
                scribble[i] = WALL_COLOR; // infinite
            else
                scribble[i] = 0;
            ++i;
        }
    }
}

Scribble::~Scribble()
{
    delete [] poscost;
    delete [] scribble;
}

bool Scribble::ValidMove(const Position &from, const Position &to) const
{
    Position p = from - (to - from);
    if (!ValidPosition(to) || !ValidPosition(from))
        return false;

    // Is it a wall on the way?
    // Is it a wall where the player would stand if pushing this box?
    if (IsWall(p) || IsWall(from) || IsWall(to))
        return false;

    return true;
}

void Scribble::BuildPositionCosts()
{
    for (unsigned int i = 0; i < size; ++i)
        poscost[i] = -1;
    // One bit for each goal reachable from this position
    for (unsigned int i = 0; i < size; ++i)
        poscost[i+size] = 0;

    // BFS from goals outwards with only valid backward moves
    // the board is filled with reachability for every goal
    // but only cost to the nearest goal
    // TODO: BFS from each goal separately and build separate cost tables for each goal
    std::deque<Position> queue;
    for (unsigned int i = 0; i < goals.size(); ++i)
    {
        queue.push_back(goals[i]);
        PosCost(goals[i]) = 0;
        int goalflag = 1<<i;
        GoalsReachable(goals[i]) |= goalflag;

        while (queue.empty() == false)
        {
            Position p = queue.front();
            queue.pop_front();
            int c = PosCost(p)+1;

			// Expand the node
			// store cost and goal reachability
            // push children into queue
            Position moves[] =
            {
                Position(p.x-1, p.y), // left
                Position(p.x+1, p.y), // right
                Position(p.x, p.y-1), // up
                Position(p.x, p.y+1)
            }; // down

            for (int j=0; j < 4; ++j)
            {
                if ((GoalsReachable(moves[j]) & goalflag) == 0 && ValidMove(moves[j], p))
                {
                    GoalsReachable(moves[j]) |= goalflag;
                    if ((unsigned int)c < (unsigned int)PosCost(moves[j]))
                        PosCost(moves[j]) = c;
                    queue.push_back(moves[j]);
                }
            }
        }
    }
}

unsigned int Scribble::ClearScribble()
{
	const int colors = 2;
    // test for overflow of scribble_color
    if (scribble_color >= ((unsigned int)-colors))
    {
        // rebuild reachable
        int size = width*height;
        for (int i = 0; i < size; ++i)
        {
            if (scribble[i] < scribble_color)
                scribble[i] = 0;
        }
        scribble_color = colors;
    }
    else
        scribble_color += colors; // Pick new colors instead of clearing
    return scribble_color;
}

void Scribble::Print() const
{
    unsigned mark_player = scribble_color-1;
    unsigned mark_box = scribble_color;

    // Print all squares
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Position p(x,y);
            unsigned int r = ColorAt(p);
            if (r > mark_box)
            {
            	if (PosCost(p) == 0)
					putchar(LOCKED_BOX);
				else
					putchar(WALL);
            }
            else if (r == mark_box)
            {
                // We have a box here
                if (PosCost(p) == 0)
                {
                    // Box on goal
                    putchar(BOX_GOAL);
                }
                else
                {
                    // Only box
                    putchar(BOX);
                }
            }
            else if (r == mark_player)
            {
                if (PosCost(p) == 0)
                    putchar(PLAYER_GOAL);
                else
                    putchar(PLAYER);
            }
            else if (PosCost(p) == 0)
            {
                // Only goal
                putchar(GOAL);
            }
            else
                putchar(FLOOR);
        }
        putchar('\n');
    }
}
