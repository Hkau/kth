#include <cstring>

#include "sokobanstate.h"
#include "sokobanlevel.h"
#include "hashqueue.h"

//#define VERBOSE_DEBUG

#ifdef VERBOSE_DEBUG
#define PRINT_VERBOSE(x, ...) printf(x, __VA_ARGS__)
#else
#define PRINT_VERBOSE(x, ...)
#endif

#define BLOCKED(pos) (scribble->Blocked(pos))
#define IS_WALL(pos) (scribble->IsWall(pos))
#define IS_BOX(pos)  (scribble->IsBox(pos))
#define POSCOST(pos) (scribble->PosCost(pos))

SokobanState::SokobanState(const SokobanLevel &level) :
        level(level),
        num_boxes(level.NumBoxes()),
        free_goals((1<<num_boxes)-1),
        player(level.GetPlayer()),
        boxes(new Position[num_boxes]),
        parent(NULL),
        last_move(0),
        last_box(-1),
        last_move_distance(0),
        scribble(level.GetScribble(free_goals))
{
    for (int i = 0; i < num_boxes; ++i)
        boxes[i] = level.GetBox(i);
    CalcCost();
}

// create new state with one new move applied
// note: (assumed to be valid, check before)
SokobanState::SokobanState(const SokobanState &state, int box, char move, int distance, int free_goals):
        level(state.level),
        num_boxes(state.num_boxes),
        free_goals(free_goals),
        player(state.player),
        boxes(new Position[num_boxes]),
        parent(&state),
        last_move(move),
        last_box(box),
        last_move_distance(distance),
        scribble(level.GetScribble(free_goals))
{
    for (int i = 0; i < num_boxes; ++i)
        boxes[i] = state.boxes[i];

	// The player is where the box was before the move
	// (gets modified for longer pushes)
    player = boxes[box];

	// Make the push and sort and make sure the boxes are sorted in the array
    switch (move)
    {
    case MOVE_UP:
        boxes[box].y -= distance;
        player.y -= distance-1;
        for (int idx = box - 1; idx >= 0; --idx)
        {
            if (boxes[idx] < boxes[box])
                break; // done, all earlier in order
            Position p = boxes[idx];
            boxes[idx] = boxes[box];
            boxes[box] = p;
            box --;
        }
        break;
    case MOVE_DOWN:
        boxes[box].y += distance;
        player.y += distance-1;
        for (int idx = box + 1; idx < num_boxes; ++idx)
        {
            if (boxes[box] < boxes[idx])
                break; // done, all earlier in order
            Position p = boxes[idx];
            boxes[idx] = boxes[box];
            boxes[box] = p;
            box ++;
        }
        break;
    case MOVE_LEFT:
        // will be sorted, or collide, but move is assumed to be valid
        boxes[box].x -= distance;
        player.x -= distance-1;
        break;
    case MOVE_RIGHT:
        // will be sorted, or collide, but move is assumed to be valid
        boxes[box].x += distance;
        player.x += distance-1;
        break;
    default:
        assert(0);
    }
    last_box = box;
    CalcCost();
}

SokobanState::~SokobanState()
{
    delete [] boxes;
    boxes = NULL;
}

/* EllipseOK tests if any of the four boxes surrounding the ellipse
 * can be pushed into the ellipse center without causing problems.
 * If so, the ellipse is not a deadlock
 */
bool SokobanState::EllipseOK(Position center) const
{
	int surround_limit = scribble->PosCost(center) == 0;

	int free_boxes = 0;

	// TODO: make additional testing to be more strict on which boxes can actually be pushed at all
	/*
	bool last_free = level.PosCost(center.x-1,center.y) > 0;
	if(last_free && IS_WALL (den bakom))
		return false; // inte ok!
	free_boxes += last_free;
	//upprepa för alla boxes
	*/

	// Given that we actually have an ellipse, it's enough to check position costs
	// because it's given that there are boxes or walls on the positions
	free_boxes += POSCOST(Position(center.x-1,center.y)) > 0;
	free_boxes += POSCOST(Position(center.x+1,center.y)) > 0;
	free_boxes += POSCOST(Position(center.x,center.y-1)) > 0;
	free_boxes += POSCOST(Position(center.x,center.y+1)) > 0;
	return free_boxes <= surround_limit;
}

void SokobanState::PrepareScribble() const
{
	scribble->ClearScribble();

    for (int i = 0; i < num_boxes; ++i)
        scribble->MarkBox(boxes[i]);

    // Mark all squares reachable by the player
    std::deque<Position> queue;
    queue.push_back(player);

    scribble->MarkReachable(player);

    while (queue.empty() == false)
    {
        Position p = queue.front();
        queue.pop_front();

        for (int i = 0; i < 4; ++i)
        {
            Position p2 = p + forward[i];
            if (scribble->IsUntouchedFloor(p2)) // Untouched floor
            {
                scribble->MarkReachable(p2);
                queue.push_back(p2);
            }
        }
    }
}

/* SpawnChildren creates new states from possible moves
 * Tries to push each box in every straight direction several steps
 * making sure the moves are valid.
 */
void SokobanState::SpawnChildren(std::vector<SokobanState*> &moves_to_add) const
{
	PrepareScribble();

	#ifdef VERBOSE_DEBUG
	Print();
	#endif

    // Try to push all boxes from every direction
    for (int i = 0; i < num_boxes; ++i)
    {
    	if (IS_WALL(boxes[i]))
    	{
    		PRINT_VERBOSE("disregards moving box %d because it is locked\n", i);
			continue;
    	}

    	SpawnChildrenFrom(i, moves_to_add);
    }
}

void SokobanState::SpawnChildrenFrom(int i, std::vector<SokobanState*> &moves_to_add) const
{
	// Loop through all the positions around the box
	for (int j = 0; j < 4; ++j)
	{
		// Make sure the player can reach the box
		if (scribble->IsReachable(boxes[i]-forward[j]) == false)
		{
			PRINT_VERBOSE("disregards moving box %d %c because the player is unable to reach\n", i, move_name[j]);
			continue;
		}

		Position p = boxes[i];

		for (int k=1;;++k)
		{
			// Try to push this box forward
			p = p + forward[j]; // The position of the box

			// Is there an obstacle or a dead spot here?
			if (BLOCKED(p))
			{
				PRINT_VERBOSE("disregards moving box %d %c %d steps because of blocking box or wall\n", i, move_name[j], k);
				break;
			}
			if (POSCOST(p) < 0)
			{
				PRINT_VERBOSE("disregards moving box %d %c %d steps because of bad position cost\n", i, move_name[j], k);
				break;
			}

			Position front = p + forward[j];
			Position frontf = front + forward[j];
			Position side1 = p + forward[(j+1)&3];
			Position side11 = side1 + forward[(j+1)&3];
			Position side11f = side11 + forward[j];
			Position side1f = side1 + forward[j];
			Position side1ff = side1f + forward[j];
			Position side1b = side1 - forward[j];
			Position side2 = p + forward[(j+3)&3];
			Position side22 = side2 + forward[(j+3)&3];
			Position side22f = side22 + forward[j];
			Position side2f = side2 + forward[j];
			Position side2ff = side2f + forward[j];
			Position side2b = side2 - forward[j];

			// Test for tunnels (push to the end)
			//    ####       ####
			// ->@$      =>    @$
			//    ####       ####
			// Not on goal and walls on the sides
			if (POSCOST(p) > 0 &&
				IS_WALL(side1) && IS_WALL(side2) &&
				IS_WALL(side1f) && IS_WALL(side2f))
			{
				PRINT_VERBOSE("disregards moving box %d %c %d steps because of tunnel\n", i, move_name[j], k);
				continue; // keep searching forwards
			}

			if (BLOCKED(front))
			{
				// Check for box-box deadlock
				// Check for 4 boxes with at least one not standing on a goal
				// Wall or box not on goal
				// **
				// *$
				if ( BLOCKED(side1) && BLOCKED(side1f) &&
					(POSCOST(p) > 0 ||
					 POSCOST(side1) > 0 ||
					 POSCOST(side1f) > 0 ||
					 POSCOST(front) > 0 ) )
				{
					PRINT_VERBOSE("disregards moving box %d %c %d steps because of 4box-deadlock\n", i, move_name[j], k);
					break;
				}
				if ( BLOCKED(side2) && BLOCKED(side2f) &&
					(POSCOST(p) > 0 ||
					 POSCOST(side2) > 0 ||
					 POSCOST(side2f) > 0 ||
					 POSCOST(front) > 0 ) )
				{
					PRINT_VERBOSE("disregards moving box %d %c %d steps because of 4box-deadlock\n", i, move_name[j], k);
					break;
				}

				// Check for S-deadlock
				//  #
				// @$$
				//   #
				if ( (POSCOST(p) > 0 || POSCOST(front) > 0) &&
					((IS_WALL(side1) && IS_WALL(side2f)) ||
					 (IS_WALL(side2) && IS_WALL(side1f))) )
				{
					PRINT_VERBOSE("disregards moving box %d %c %d steps because of S-deadlock\n", i, move_name[j], k);
					break;
				}

				// Check for S-deadlock
				//  #
				//  $$
				//  @#
				if (IS_WALL(front))
				{
					if ( BLOCKED(side1) && IS_WALL(side1b) &&
						(POSCOST(p) > 0 || POSCOST(side1) > 0) )
					{
						PRINT_VERBOSE("disregards moving box %d %c %d steps because of S-deadlock\n", i, move_name[j], k);
						break;
					}
					if ( BLOCKED(side2) && IS_WALL(side2b) &&
						(POSCOST(p) > 0 || POSCOST(side2) > 0) )
					{
						PRINT_VERBOSE("disregards moving box %d %c %d steps because of S-deadlock\n", i, move_name[j], k);
						break;
					}
				}
			}

			// Check for "elliptic" deadlock
			// Corner deadlock is a special case of elliptic deadlock
			// $$                     ###
			// $ $                    # $
			//  $$                    #$
			if (BLOCKED(front))
			{
				// Test for corner position
				// @$$
				//  $ $
				//   $$
				if (level.ValidPosition(side1f+(side1f-p)) &&
						!BLOCKED(side1f) &&
						BLOCKED(side1) &&
						BLOCKED(side1f+(side1-p)) &&
						BLOCKED(side1f+(side1f-p)) &&
						BLOCKED(side1f+(front-p)) &&
						(!EllipseOK(side1f) || (POSCOST(p) != 0 && POSCOST(side1f) != 0)))
				{
					PRINT_VERBOSE("disregards moving box %d %c %d steps because of elliptic deadlock\n", i, move_name[j], k);
					break;
				}

				if (level.ValidPosition(side2f+(side2f-p)) &&
						!BLOCKED(side2f) &&
						BLOCKED(side2) &&
						BLOCKED(side2f+(side2-p)) &&
						BLOCKED(side2f+(side2f-p)) &&
						BLOCKED(side2f+(front-p)) &&
						(!EllipseOK(side2f) || (POSCOST(side2f) != 0 && POSCOST(p) != 0)))
				{
					PRINT_VERBOSE("disregards moving box %d %c %d steps because of elliptic deadlock\n", i, move_name[j], k);
					break;
				}
				// Test for
				//  $$
				//  $ $
				//->@$$
				if (level.ValidPosition(side1+(side1f-p)) &&
						!BLOCKED(side1) &&
						BLOCKED(side1-(front-p)) &&
						BLOCKED(side1f) &&
						BLOCKED(side1+(side1-p)) &&
						BLOCKED(side1+(side1-p)-(front-p)) &&
						(!EllipseOK(side1) ))
				{
					PRINT_VERBOSE("disregards moving box %d %c %d steps because of elliptic deadlock\n", i, move_name[j], k);
					break;
				}
				if (level.ValidPosition(side2+(side2f-p)) &&
						!BLOCKED(side2) &&
						BLOCKED(side2-(front-p)) &&
						BLOCKED(side2f) &&
						BLOCKED(side2+(side2-p)) &&
						BLOCKED(side2+(side2-p)-(front-p)) &&
						(!EllipseOK(side2) ))
				{
					PRINT_VERBOSE("disregards moving box %d %c %d steps because of elliptic deadlock\n", i, move_name[j], k);
					break;
				}
			}

			// Test for center position
			//  $$
			// @$ $
			//   $$
			if (level.ValidPosition(front+(front-p)) &&
					!BLOCKED(front) &&
					BLOCKED(front+(front-p)) &&
					BLOCKED(side1f) &&
					BLOCKED(side2f) )
			{
				if (BLOCKED(side1) &&
						BLOCKED(side2f+(front-p)) &&
						!EllipseOK(front))
				{
					PRINT_VERBOSE("disregards moving box %d %c %d steps because of elliptic deadlock\n", i, move_name[j], k);
					break;
				}
				if (BLOCKED(side2) &&
						BLOCKED(side1f+(front-p)) &&
						!EllipseOK(front))
				{
					PRINT_VERBOSE("disregards moving box %d %c %d steps because of elliptic deadlock\n", i, move_name[j], k);
					break;
				}
			}

			// Check for diamond deadlock
			//   #
			//  #-#
			//  -$-
			if (POSCOST(p) != 0 &&
				POSCOST(front) < 0 && POSCOST(side1) < 0 && POSCOST(side2) < 0 &&
				IS_WALL(frontf) && IS_WALL(side1f) && IS_WALL(side2f) )
			{
				PRINT_VERBOSE("disregards moving box %d %c %d steps because of diamond deadlock\n", i, move_name[j], k);
				break;
			}

			int new_free_goals = free_goals;

			if (POSCOST(p) == 0) // On goal
			{
				// If this box is immobilized over a goal
				// Make sure that every free box can reach a free goal
				// @*#
				//  #
				if ( IS_WALL(front) && (IS_WALL(side1) || IS_WALL(side2)))
					new_free_goals &= ~(1<<level.GetGoalAt(p));

				// @**
				//  **
				if ( BLOCKED(front) && POSCOST(front) <= 0 &&
					  ((BLOCKED(side1) && POSCOST(side1) <= 0 &&
						BLOCKED(side1f) && POSCOST(side1f) <= 0) ||
					   (BLOCKED(side2) && POSCOST(side2) <= 0 &&
						BLOCKED(side2f) && POSCOST(side2f) <= 0)) )
				{
					new_free_goals &= ~(1<<level.GetGoalAt(p)) & ~(1<<level.GetGoalAt(front));
					if (BLOCKED(side1) && POSCOST(side1) <= 0 &&
							BLOCKED(side1f) && POSCOST(side1f) <= 0)
					{
						new_free_goals &= ~(1<<level.GetGoalAt(side1));
						new_free_goals &= ~(1<<level.GetGoalAt(side1f));
						if (level.ValidPosition(frontf) && BLOCKED(frontf) && POSCOST(frontf) <= 0 &&
								level.ValidPosition(side1ff) && BLOCKED(side1ff) && POSCOST(side1ff) <= 0)
							new_free_goals &= ~(1<<level.GetGoalAt(frontf)) & ~(1<<level.GetGoalAt(side1ff));

						if (level.ValidPosition(side11) && BLOCKED(side11) && POSCOST(side11) <= 0 &&
								level.ValidPosition(side11f) && BLOCKED(side11f) && POSCOST(side11f) <= 0)
							new_free_goals &= ~(1<<level.GetGoalAt(side11)) & ~(1<<level.GetGoalAt(side11f));
					}
					if (BLOCKED(side2) && POSCOST(side2) <= 0 &&
							BLOCKED(side2f) && POSCOST(side2f) <= 0)
					{
						new_free_goals &= ~(1<<level.GetGoalAt(side2));
						new_free_goals &= ~(1<<level.GetGoalAt(side2f));

						if (level.ValidPosition(frontf) && BLOCKED(frontf) && POSCOST(frontf) <= 0 &&
								level.ValidPosition(side2ff) && BLOCKED(side2ff) && POSCOST(side2ff) <= 0)
							new_free_goals &= ~(1<<level.GetGoalAt(frontf)) & ~(1<<level.GetGoalAt(side2ff));

						if (level.ValidPosition(side22) && BLOCKED(side22) && POSCOST(side22) <= 0 &&
								level.ValidPosition(side22f) && BLOCKED(side22f) && POSCOST(side22f) <= 0)
							new_free_goals &= ~(1<<level.GetGoalAt(side22)) & ~(1<<level.GetGoalAt(side22f));
					}

					bool bad_move = false;
					for (int b=0; b < num_boxes; ++b)
					{
						if (b==i)
							continue;
						if ( POSCOST(boxes[b]) != 0 &&
								(scribble->GoalsReachable(boxes[b]) & new_free_goals) == 0 )
						{
							Position delta = p-boxes[i];
							PRINT_VERBOSE("box %d will be unable to reach any goal if box %d is moved (%d,%d)\n", b, i, delta.x, delta.y);
							// this is not an ok move
							bad_move = true;
							break;
						}
					}
					if (bad_move)
						break;
				}
			}

			SokobanState *s = new SokobanState(*this, i, move_name[j], k, new_free_goals);
			if (s->Cost() == unsigned(-1))
			{
				PRINT_VERBOSE("disregards moving box %d %c %d steps because of negative cost of new  state\n", i, move_name[j], k);
				delete s;
				s = NULL;
				break;
			}

			moves_to_add.push_back(s);

			PRINT_VERBOSE("added move of box %d %c %d steps\n", i, move_name[j], k);
		}
	}
}

/* Moves builds a string of the moves to walk from one position
 * to another without moving any box
 */
std::string SokobanState::Moves(Position from, Position to) const
{
    int width = level.GetWidth(), height = level.GetHeight();
    char reachable[width*height];
    memset(reachable, FLOOR, sizeof(char)*width*height);

    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            if (level.GetMapPos(x, y) == WALL)
                reachable[y*width + x] = WALL;

    for (int i = 0; i < num_boxes; ++i)
        reachable[boxes[i].y * width + boxes[i].x] = BOX;

    // Make markings of the directions needed to reach goal
    std::deque<Position> queue;
    queue.push_back(to);
    reachable[to.y * width + to.x] = PLAYER;

    while (queue.empty() == false)
    {
        Position p = queue.front();
        queue.pop_front();

        for (int i = 0; i < 4; ++i)
        {
            Position p2 = p - forward[i];
            if (reachable[p2.y * width + p2.x] == FLOOR)
            {
                reachable[p2.y * width + p2.x] = move_name[i];
                if (p2 == from)
                {
                    queue.clear();
                    break;
                }
                queue.push_back(p2);
            }
        }
    }

    Position p = from;
    assert(reachable[p.y * width + p.x] != FLOOR);

	// Follow the markings
    std::string movestr;
    while (p != to)
    {
        char move = reachable[p.y * width + p.x];
        movestr += move;

        switch (move)
        {
        case MOVE_UP:
            p.y--;
            break;
        case MOVE_DOWN:
            p.y++;
            break;
        case MOVE_LEFT:
            p.x--;
            break;
        case MOVE_RIGHT:
            p.x++;
            break;
        default:
            putchar(move);
            putchar('\n');
            assert("invalid move" == 0);
        }
    }
    return movestr;
}

/* Moves builds a string of all the player moves from start state to this state
 * Moves generates a solution string if this state is a solution
 */
std::string SokobanState::Moves() const
{
    //Print();
    if (parent == NULL)
        return std::string("");
    assert(last_move_distance >= 0);

    std::string push;
    for (int i = 0; i < last_move_distance; ++i)
        push += last_move;

    Position from = parent->player;
    Position to = player;

    switch (last_move)
    {
    case(MOVE_UP):
                    to.y += last_move_distance;
        break;
    case(MOVE_DOWN):
                    to.y -= last_move_distance;
        break;
    case MOVE_LEFT:
        to.x += last_move_distance;
        break;
    case MOVE_RIGHT:
        to.x -= last_move_distance;
        break;
    default:
        assert(0);
    }

    return parent->Moves() + parent->Moves(from, to) + push;
}

bool SokobanState::operator==(const SokobanState &b) const
{
	// Two states are considered equal if all the boxes are on the same positions
	// and there exists a path between player positions

    if (cost != b.cost)
        return false;

    if (num_boxes != b.num_boxes)
        return false;

    for (int i = 0; i < b.num_boxes; ++i)
        if (boxes[i] != b.boxes[i])
            return false;

    if (player == b.player)
		return true;

	// Search for a path between player positions
	scribble->ClearScribble();

	for (int i = 0; i < num_boxes; ++i)
		scribble->MarkBox(boxes[i]);

	// Mark all squares reachable by the player
	std::priority_queue<PriorityQueue<Position>::Post> queue;
	queue.push(PriorityQueue<Position>::Post(player, player.DistanceTo(b.player)));
	//PriorityQueue<Position> queue;
	//queue.Push(player, player.DistanceTo(b.player));
	//std::deque<Position> queue;
	//queue.push_back(player);

	scribble->MarkReachable(player);

	while (queue.empty() == false)
	{
		Position p = queue.top().elem;
		queue.pop();
		//Position p = queue.Pop();
		//Position p = queue.front();
		//queue.pop_front();

		for (int i = 0; i < 4; ++i)
		{
			Position p2 = p + forward[i];
			if (scribble->IsUntouchedFloor(p2)) // == FLOOR
			{
				// If player a can reach player b, the states are equal
				if (p2 == b.player)
					return true;

				scribble->MarkReachable(p2); // = PLAYER
				queue.push(PriorityQueue<Position>::Post(p2, p2.DistanceTo(b.player)));
				//queue.Push(p2, p2.DistanceTo(b.player));
				//queue.push_back(p2);
			}
		}
	}
	// Return false if no path was found
	return false;
}

void SokobanState::CalcCost()
{
	// TODO: Test if there exists a bipartite matching of goals and boxes

	// For every goal previously reachable by this box,
	// make sure there still is some box that can reach the goal
	int goals_left = (1<<num_boxes)-1;
	for (int i = 0; i < num_boxes; ++i)
	{
		// Add the boxes reachable from this position to the list of reachable goals
		goals_left &= ~(scribble->GoalsReachable(boxes[i]));
		if (goals_left == 0)
			break;
	}

	// If there is a goal we cant reach with any box, the state is useless
	if (goals_left != 0)
	{
		cost = unsigned(-1);
		return;
	}

    cost = 0;
    for (int i = 0; i < num_boxes; ++i)
    {
        int pos_cost = POSCOST(boxes[i]);
		if (pos_cost < 0)
		{
			cost = unsigned(-1);
			return;
		}
        if (pos_cost > 0)
            cost += pos_cost + 100;
		else
		{
			// Make goals next to walls slightly more attractive
			if (!IS_WALL(boxes[i]))
				cost += 1;
			for (int j=0; j<4; j++)
			{
				if (!IS_WALL(boxes[i]+forward[j]))
					cost += 1;
			}
		}
    }
}

bool SokobanState::IsSolution() const
{
    for (int i = 0; i < num_boxes; ++i)
		if (POSCOST(boxes[i]) != 0 )
			return false;
	return true;
}

void SokobanState::Print() const
{
    // Print all squares
    int width = level.GetWidth();
    int height = level.GetHeight();
    int b = 0;
    int g = 0;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Position p(x,y);
            while (b < num_boxes && boxes[b] < p)
                ++b;
            while (g < num_boxes && level.GetGoal(g) < p)
                ++g;
			if (g < num_boxes && level.GetGoal(g) == p && (((1<<g) & free_goals) == 0))
			{
				putchar(LOCKED_BOX);
			}
            else if (b < num_boxes && boxes[b] == p)
            {
                // We have a box here
                if (POSCOST(p) == 0)
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
            else if (p == player)
            {
                if (POSCOST(p) == 0)
                    putchar(PLAYER_GOAL);
                else
                    putchar(PLAYER);
            }
            else if (level.GetMapPos(p) == WALL)
                putchar(WALL);
            else if (POSCOST(p) == 0)
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
