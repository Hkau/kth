/*
 * StaticBoard.h
 *
 *  Created on: Oct 14, 2010
 *      Author: soheil
 */

#ifndef STATICBOARD_H_
#define STATICBOARD_H_
#include <sys/types.h>
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <set>
using namespace std;
#define S_T(tt) (1 << tt##_)

#define IS(c,tt)  (c & S_T(tt))
#define ADD(c,tt) (c |= S_T(tt))
#define REM(c,tt) (c &= (~S_T(tt)))

enum square_type
{
	wall_ = 0, box_=1, sokoban_=2,reachable_=3,goal_=4,h_ddlock_=5,s_ddlock_=6,g_ddlock=7
};
enum direction
{
	UP , DOWN, RIGHT, LEFT, NONE
};
/*
 * Board related type
 */
typedef u_int8_t sq_t;
typedef u_int8_t pos_t;
typedef pair<pos_t,pos_t> coord_t;

/*
 * Board related util functions
 */
ostream& operator<<(ostream&os, const coord_t&s);
ostream& operator<<(ostream&os, const vector<vector<sq_t> >&sb);
ostream& operator<<(ostream&os, const direction&d);

char interpret(sq_t s, bool flag = true);
bool reachableNeighbor(coord_t box_p, coord_t&r, vector<vector<sq_t> >&b);
vector<coord_t> flood_fill(vector<vector<sq_t> >&B, coord_t&s, int am = 0, ...);
vector<vector<double> >Reachability2Graph(const vector< vector<set<pos_t> > >&r, pos_t ti);
void graph_node_rank(vector<vector<double> >&g, const coord_t src, const int iteration=100);

class StaticBoard {
public:
	StaticBoard(const char*str);

	void calculate_goal_reachability();
	void calculate_goal_i_reachability();
	void calculate_goal_box_reachability();
	void fixed(pos_t goal_index, pos_t box_index);

	pos_t X() const;
	pos_t Y() const;

	vector< vector<sq_t> > board() const;
	vector< vector<pos_t> >goal_i_index() const;
	vector< vector<pos_t> >box_i_index() const;

	vector< coord_t> goal_pos() const;
	vector< coord_t> box_pos() const;
	vector< coord_t> box_initial_pos() const;

	coord_t sok_pos() const;

	vector<vector<vector<size_t> > > goal_reachability() const;
	vector<vector<vector<size_t> > > goal_i_reachability() const;

	vector<vector<size_t> > goal_box_reachability() const;

	pos_t num_;
private:
	StaticBoard();
	void mark_deadlocks();
	pos_t _X,_Y;

	vector< vector<sq_t> >_board;
	vector< vector<pos_t> >_goal_i_index;
	vector< vector<pos_t> >_box_i_index;

	vector< coord_t> _goal_pos;
	vector< coord_t> _box_pos;

	coord_t _sok_pos;

	vector<vector<vector<size_t> > >_goal_reachability;
	vector<vector<vector<size_t> > >_goal_i_reachability;

	vector<vector<size_t> > _goal_box_reachability;
	map<pos_t,pos_t> _goal_box_map;
};
ostream& operator<<(ostream&os, const StaticBoard&sb);

/*
 * inline Methods
 */
inline pos_t StaticBoard::X() const {
	return _X;
}
inline pos_t StaticBoard::Y() const {
	return _Y;
}

inline vector<vector<sq_t> > StaticBoard::board() const {
	return _board;
}
inline vector<vector<pos_t> > StaticBoard::goal_i_index() const {
	return _goal_i_index;
}
inline vector<vector<pos_t> > StaticBoard::box_i_index() const {
	return _box_i_index;
}

inline vector<coord_t> StaticBoard::goal_pos() const {
	return _goal_pos;
}
inline vector<coord_t> StaticBoard::box_pos() const {
	return _box_pos;
}
inline vector<coord_t> StaticBoard::box_initial_pos() const {
	return _box_pos;
}

inline coord_t StaticBoard::sok_pos() const {
	return _sok_pos;
}

inline vector<vector<vector<size_t> > > StaticBoard::goal_reachability() const {
	return _goal_reachability;
}

inline vector<vector<vector<size_t> > > StaticBoard::goal_i_reachability() const {
	return _goal_i_reachability;
}

inline vector<vector<size_t> > StaticBoard::goal_box_reachability() const {
	return _goal_box_reachability;
}


#endif /* STATICBOARD_H_ */
