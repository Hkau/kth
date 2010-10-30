/*
 * Board.h
 *
 *  Created on: Oct 16, 2010
 *      Author: soheil
 */

#ifndef BOARD_H_
#define BOARD_H_
#include <list>
#include "StaticBoard.h"

typedef pair<pos_t, direction> action_t;

#define NO_ACTION make_pair(-1, NONE)
ostream& operator<<(ostream& os, const action_t&a);
class Board {
public:
	friend ostream& operator<<(ostream& os, const Board&b);
	const pos_t X, Y;
	const bool forward;
	Board(StaticBoard*p, bool f);
	Board(const Board&ob);
	vector<vector<pos_t> > goal_i_index() const;
	vector<coord_t> goal_pos() const;
	vector<coord_t> box_pos() const;
	bool is_valid_action(action_t a);
	bool make_action(action_t a);
	coord_t get_sok()const;
	vector<action_t> pos_action();
	size_t hash() const;
	StaticBoard*get_static_board() const;
	void freeze(coord_t pos);
	void unfreeze(coord_t pos);
	bool operator==(const Board& ob) const;
	virtual ~Board();
private:
/*
 * State
 */
	vector<vector<sq_t> > _board;
	StaticBoard*sb;
	vector<coord_t> m_pos;

	coord_t sok;

/*
 * Temp
 */
	coord_t sok_des;
	coord_t to_be_free;
	friend list<direction> Multi2SingleAgent(list<action_t> ma_a, StaticBoard*s);
};
inline coord_t Board::get_sok()const{
	return sok;
}
inline vector<vector<pos_t> > Board::goal_i_index() const {
	return forward ? sb->goal_i_index() : sb->box_i_index();
}
inline vector<coord_t> Board::goal_pos() const {
	return forward ? sb->goal_pos() : sb->box_pos();
}
inline vector<coord_t> Board::box_pos() const {
	return m_pos;
}
inline StaticBoard* Board::get_static_board() const{
	return sb;
}
#endif /* BOARD_H_ */
