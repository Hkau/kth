#ifndef SOKOBANSTATE_H
#define SOKOBANSTATE_H

#include <cassert>
#include <string>

#include "lookup3.h"
#include "position.h"
#include "sokobanlevel.h"
#include "priorityqueue.h"

//#define REVERSE_SOLVE

static const char MOVE_RIGHT	= 'R';
static const char MOVE_UP		= 'U';
static const char MOVE_LEFT		= 'L';
static const char MOVE_DOWN		= 'D';

// In counter-clockwise direction to simplify left/right turns
static const Position forward[] = {
	Position(1,0),
	Position(0,-1),
	Position(-1,0),
	Position(0,1)};

static const char move_name[] = {
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_LEFT,
	MOVE_DOWN};

class SokobanState
{
private:
	// A reference to the level to ask for a scribble
	const SokobanLevel &level;

	const int num_boxes;

	// Bitmask of goals without immobilized boxes on them
	const int free_goals;

	Position player;
	Position *boxes;

	// Cost for the state, determines the order the states are poped from the queue
	unsigned int cost;

	// History
	const SokobanState *parent;
	const char last_move;
	int last_box;
	const int last_move_distance;

	// Object to make temporary markings in and ask for position costs and goal reachability
	Scribble *scribble;

	// Regular copy constructor is not allowed
	SokobanState(const SokobanState &state);
	void operator=(const SokobanState &state);

	/* EllipseOK tests if any of the four boxes surrounding the ellipse
	 * can be pushed into the ellipse center without causing problems.
	 * If so, the ellipse is not a deadlock
	 */
	bool EllipseOK(Position center) const;
	void PrepareScribble() const;

	/* SpawnChildren creates new states from possible moves
	 * Tries to push each box in every straight direction several steps
	 * making sure the moves are valid.
	 */
	void SpawnChildrenFrom(int box, std::vector<SokobanState*> &moves_to_add) const;
	void CalcCost();

public:
	SokobanState(const SokobanLevel &level);
	// create new state with one new move applied
	// note: (assumed to be valid, check before)
	SokobanState(const SokobanState &state, int box, char move, int distance, int free_goals);

	~SokobanState();

	/* SpawnChildren creates new states from possible moves
	 * Tries to push each box in every straight direction several steps
	 * making sure the moves are valid.
	 */
	void SpawnChildren(std::vector<SokobanState*> &moves_to_add) const;

	/* Moves builds a string of the moves to walk from one position
	 * to another without moving any box
	 */
	std::string Moves(Position from, Position to) const;

	/* Moves builds a string of all the player moves from start state to this state
	 * Moves generates a solution string if this state is a solution
	 */
	std::string Moves() const;

	unsigned int Cost() const
	{	return cost;	}

	bool IsSolution() const;
	void Print() const;

	bool operator==(const SokobanState &b) const;

	friend bool Equal(const SokobanState *a, const SokobanState *b)
	{
		return *a == *b;
	}

	friend uint32_t Hash(const SokobanState *s)
	{
		return hashword( (uint32_t*) s->boxes, (s->num_boxes<<1), 0);
	}
};

#endif // SOKOBANSTATE_H
