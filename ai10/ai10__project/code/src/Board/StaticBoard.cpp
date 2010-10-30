/*
 * StaticBoard.cpp
 *
 *  Created on: Oct 14, 2010
 *      Author: soheil
 */

#include <cstring>
#include <stdio.h>
#include <stdarg.h>
#include <stack>
#include <queue>
#include "../util.h"
#include "StaticBoard.h"
#include "BoardLocal.h"

vector<coord_t>
flood_fill(vector<vector<sq_t> >&B, coord_t&s, vector<sq_t>&blk);
StaticBoard::StaticBoard(const char*str) :
	_X((char*) memchr(str, '\n', strlen(str)) - str), _Y((strlen(str) - 1)
			/ (_X + 1)), _board(_X, vector<sq_t> (_Y, sq_t(0))), _goal_i_index(
			_X, vector<pos_t> (_Y, pos_t(-1))), _box_i_index(_X, vector<pos_t> (
			_Y, pos_t(-1))), _goal_reachability(_X, vector<vector<size_t> > (_Y,
			vector<size_t> ())), _goal_i_reachability(_X, vector<vector<size_t> > (_Y,
			vector<size_t> ())) {

	/*
	 * Start of function!!
	 */
	for (pos_t y = 0; y < _Y; y++) {
		for (pos_t x = 0; x < _X; x++) {
			const char c = str[y * (_X + 1) + x];
			switch (c) {
			case '#':
				ADD(_board[x][y],wall);
				break;
			case '@':
				ADD(_board[x][y],sokoban);
				_sok_pos = coord_t(x, y);
				break;
			case '+':
				ADD(_board[x][y],sokoban);
				ADD(_board[x][y],goal);
				_sok_pos = coord_t(x, y);
				_goal_i_index[x][y] = _goal_pos.size();
				_goal_pos.push_back(coord_t(x, y));
				break;
			case '$':
				ADD(_board[x][y],box);
				_box_i_index[x][y] = _box_pos.size();
				_box_pos.push_back(coord_t(x, y));
				break;
			case '*':
				ADD(_board[x][y],box);
				ADD(_board[x][y],goal);

				_goal_i_index[x][y] = _goal_pos.size();
				_goal_pos.push_back(coord_t(x, y));
				_box_i_index[x][y] = _box_pos.size();
				_box_pos.push_back(coord_t(x, y));
				break;
			case '.':
				ADD(_board[x][y],goal);

				_goal_i_index[x][y] = _goal_pos.size();
				_goal_pos.push_back(coord_t(x, y));
				break;
			}
		}
	}
	num_ = _goal_pos.size();
	mark_deadlocks();
	flood_fill(_board, _sok_pos, 0);
}

void StaticBoard::calculate_goal_reachability() {
	vector<vector<sq_t> > bare_board = _board;
	queue<pair<coord_t, pair<pos_t, size_t> > > to_process;
	for (pos_t y_i = 0; y_i < _Y ; y_i++) {
		for (pos_t x_i = 0; x_i < _X ; x_i++) {
			REM(bare_board[x_i][y_i], box);
			_goal_reachability[x_i][y_i] = vector<size_t>(num_, 0);
			if (IS(_board[x_i][y_i], goal)) {
				to_process.push(make_pair(make_pair(x_i, y_i),
						make_pair(_goal_i_index[x_i][y_i],0)));
			}
		}
	}

	while (!to_process.empty()) {
#define M_REACH_ITER _X*_Y
#define INX(x) (((x)>=0)&&((x)<_X))
#define INY(y) (((y)>=0)&&((y)<_Y))
#define CHECK_AND_PUSH \
		if (INX(des_sok.first) && INY(des_sok.second) && _goal_reachability[box_p.first][box_p.second][g_id] == 0) {\
			ADD(bare_board[box_p.first][box_p.second], box);\
			flood_fill(bare_board, _sok_pos, 0);\
			if (!IS(bare_board[to_be_free.first][to_be_free.second], wall))\
			if (IS(bare_board[box_p.first][box_p.second], reachable)){\
				reachableNeighbor(box_p, nearest_sok, bare_board);\
				if(IS(bare_board[des_sok.first][des_sok.second], reachable) || !searchPath(nearest_sok,box_p,des_sok,box_p, bare_board, M_REACH_ITER).empty()){\
					to_process.push(make_pair(box_p, make_pair(g_id, _goal_reachability[x_i][y_i][g_id])));\
				}\
			}\
			REM(bare_board[box_p.first][box_p.second], box);\
		}
		////////////////////////////////////////

		pos_t x_i = to_process.front().first.first;
		pos_t y_i = to_process.front().first.second;
		pos_t g_id = to_process.front().second.first;
		size_t p_len = to_process.front().second.second;

		to_process.pop();

		if (IS(_board[x_i][y_i], wall))
			continue;
		if ((IS(_board[x_i][y_i], h_ddlock)) && !IS(_board[x_i][y_i], goal))
			continue;

		coord_t box_p;
		coord_t des_sok;
		coord_t to_be_free;
		coord_t nearest_sok;

		_goal_reachability[x_i][y_i][g_id]=p_len+1;

		/*
		 * Can come from left
		 */
		box_p = make_pair(x_i - 1, y_i);
		des_sok = make_pair(x_i - 2, y_i);
		to_be_free = make_pair(x_i, y_i);
		CHECK_AND_PUSH;
		/*
		 * Can come from right
		 */
		box_p = make_pair(x_i + 1, y_i);
		des_sok = make_pair(x_i + 2, y_i);
		to_be_free = make_pair(x_i, y_i);
		CHECK_AND_PUSH;
		/*
		 * Can come from top
		 */
		box_p = make_pair(x_i, y_i - 1);
		des_sok = make_pair(x_i, y_i - 2);
		to_be_free = make_pair(x_i, y_i);
		CHECK_AND_PUSH;
		/*
		 * Can come from top
		 */
		box_p = make_pair(x_i, y_i + 1);
		des_sok = make_pair(x_i, y_i + 2);
		to_be_free = make_pair(x_i, y_i);
		CHECK_AND_PUSH;
	}
}
void StaticBoard::calculate_goal_i_reachability() {
	vector<vector<sq_t> > bare_board = _board;
	queue<pair<coord_t, pair<pos_t, size_t> > > to_process;
	for (pos_t y_i = 0; y_i < _Y ; y_i++) {
		for (pos_t x_i = 0; x_i < _X ; x_i++) {
			REM(bare_board[x_i][y_i], goal);
			_goal_i_reachability[x_i][y_i] = vector<size_t>(num_, 0);
			if (IS(_board[x_i][y_i], box)) {
				to_process.push(make_pair(make_pair(x_i, y_i),
						make_pair(_box_i_index[x_i][y_i], 0)));
			}
		}
	}

	while (!to_process.empty()) {
#define INX(x) (((x)>=0)&&((x)<_X))
#define INY(y) (((y)>=0)&&((y)<_Y))
#define CHECK_AND_PULL \
		if (INX(des_sok.first) && INY(des_sok.second) && _goal_i_reachability[box_p.first][box_p.second][g_id] == 0) {\
			if ((!IS(bare_board[to_be_free.first][to_be_free.second], wall))&&(!IS(bare_board[box_p.first][box_p.second], wall))){\
			ADD(bare_board[box_p.first][box_p.second], box);\
			flood_fill(bare_board, _sok_pos, 0);\
			{\
				if(IS(bare_board[des_sok.first][des_sok.second], reachable)){\
					to_process.push(make_pair(box_p, make_pair(g_id, _goal_i_reachability[x_i][y_i][g_id])));\
				}\
			}\
			REM(bare_board[box_p.first][box_p.second], box);\
			}\
		}
		////////////////////////////////////////

		pos_t x_i = to_process.front().first.first;
		pos_t y_i = to_process.front().first.second;
		pos_t g_id = to_process.front().second.first;
		size_t p_len = to_process.front().second.second;
		to_process.pop();

		if (IS(_board[x_i][y_i], wall))
			continue;

		coord_t box_p;
		coord_t des_sok;
		coord_t to_be_free;
		coord_t nearest_sok;

		_goal_i_reachability[x_i][y_i][g_id]=p_len+1;

		/*
		 * Can come from left
		 */
		box_p = make_pair(x_i - 1, y_i);
		des_sok = make_pair(x_i, y_i);
		to_be_free = make_pair(x_i + 1, y_i);
		CHECK_AND_PULL;
		/*
		 * Can come from right
		 */
		box_p = make_pair(x_i + 1, y_i);
		des_sok = make_pair(x_i, y_i);
		to_be_free = make_pair(x_i - 1, y_i);
		CHECK_AND_PULL;
		/*
		 * Can come from top
		 */
		box_p = make_pair(x_i, y_i - 1);
		des_sok = make_pair(x_i, y_i);
		to_be_free = make_pair(x_i, y_i + 1);
		CHECK_AND_PULL;
		/*
		 * Can come from top
		 */
		box_p = make_pair(x_i, y_i + 1);
		des_sok = make_pair(x_i, y_i);
		to_be_free = make_pair(x_i, y_i - 1);
		CHECK_AND_PULL;
		//printSetMatrix(cerr, goal_i_reachability(), box_pos()) << endl;
	}
}
/*
 * You have to make sure that goal reachability is calculated before
 */
void StaticBoard::calculate_goal_box_reachability() {
	_goal_box_reachability = vector<vector<size_t> >(num_, vector<size_t>(num_, ~0));
	for(size_t g=0;g<num_;g++)
		for(size_t b=0;b<num_;b++)
			_goal_box_reachability[g][b] = _goal_reachability[_box_pos[b].first][_box_pos[b].second][g];

}
void StaticBoard::fixed(pos_t goal_index, pos_t box_index) {
}

void StaticBoard::mark_deadlocks() {
	for (pos_t y = 1; y < _Y - 1; y++) {
		for (pos_t x = 1; x < _X - 1; x++) {
			if (IS(_board[x][y],wall) | IS(_board[x][y],h_ddlock)
					|IS(_board[x][y],s_ddlock))
				continue;
			/*
			 * TOP LEFT
			 */
			if (IS(_board[x-1][y],wall) && IS(_board[x-1][y-1],wall)
					&& IS(_board[x][y-1],wall)) {
				ADD(_board[x][y],h_ddlock);
			}
			/*
			 * TOP RIGHT
			 */
			if (IS(_board[x+1][y],wall) && IS(_board[x+1][y-1],wall)
					&& IS(_board[x][y-1],wall)) {
				ADD(_board[x][y],h_ddlock);
				bool topwall = true;
				for (pos_t _x = x - 1; !(IS(_board[_x][y],wall)); _x--) {
					if (IS(_board[_x][y],h_ddlock))
						break;
					if (!IS(_board[_x][y-1],wall) || IS(_board[_x][y],goal)) {
						topwall = false;
						break;
					}
				}
				if (topwall) {
					for (pos_t _x = x - 1; !(IS(_board[_x][y],wall)
							||IS(_board[_x][y],h_ddlock)); _x--) {
						ADD(_board[_x][y],s_ddlock);
					}
				}
			}
			/*
			 * BOT LEFT
			 */
			if (IS(_board[x-1][y],wall) && IS(_board[x-1][y+1],wall)
					&& IS(_board[x][y+1],wall)) {
				ADD(_board[x][y],h_ddlock);
				bool leftwall = true;
				for (pos_t _y = y - 1; !(IS(_board[x][_y],wall)); _y--) {
					if (IS(_board[x][_y],h_ddlock))
						break;
					if (!IS(_board[x-1][_y],wall) || IS(_board[x][_y],goal)) {
						leftwall = false;
						break;
					}
				}
				if (leftwall) {
					for (pos_t _y = y - 1; !(IS(_board[x][_y],wall)
							|| IS(_board[x][_y],h_ddlock)); _y--) {
						ADD(_board[x][_y],s_ddlock);
					}
				}

			}
			/*
			 * BOT RIGHT
			 */
			if (IS(_board[x+1][y],wall) && IS(_board[x+1][y+1],wall)
					&& IS(_board[x][y+1],wall)) {
				ADD(_board[x][y],h_ddlock);
				bool botwall = true;
				for (pos_t _x = x - 1; !(IS(_board[_x][y],wall)); _x--) {
					if (IS(_board[_x][y],h_ddlock))
						break;
					if (!IS(_board[_x][y+1],wall) || IS(_board[_x][y],goal)) {
						botwall = false;
						break;
					}
				}
				if (botwall) {
					for (pos_t _x = x - 1; !(IS(_board[_x][y],wall)
							||IS(_board[_x][y],h_ddlock)); _x--) {
						ADD(_board[_x][y],s_ddlock);
					}
				}
				bool rightwall = true;
				for (pos_t _y = y - 1; !(IS(_board[x][_y],wall)); _y--) {
					if (IS(_board[x][_y],h_ddlock))
						break;
					if (!IS(_board[x+1][_y],wall) || IS(_board[x][_y],goal)) {
						rightwall = false;
						break;
					}
				}
				if (rightwall) {
					for (pos_t _y = y - 1; !(IS(_board[x][_y],wall)
							|| IS(_board[x][_y],h_ddlock)); _y--) {
						ADD(_board[x][_y],s_ddlock);
					}
				}

			}

		}
	}
}
/*
 * Util Functions
 */
vector<vector<double> > Reachability2Graph(
		const vector<vector<set<pos_t> > >&r, pos_t ti) {
	pos_t Y(r[0].size());
	pos_t X(r.size());
	vector<vector<double> > g(X, vector<double> (Y, 0));
	for (size_t y = 0; y < Y; y++) {
		for (size_t x = 0; x < X; x++) {
			if (r[x][y].count(ti) > 0)
				g[x][y] = 1;
		}
	}
	return g;
}
void graph_node_rank(vector<vector<double> >&g, const coord_t src,const int iteration) {

#define NODE_RANK(XX,YY) (g[XX-1][YY]+g[XX+1][YY]+g[XX][YY-1]+g[XX][YY+1])
#define PRESENT(XX,YY) (g[XX][YY]>0?1:0)

#define NODE_L(XX,YY) (PRESENT(XX-1,YY) + PRESENT(XX+1,YY) + PRESENT(XX,YY-1) + PRESENT(XX,YY+1) )

	pos_t Y(g[0].size());
	pos_t X(g.size());
	double total;
	vector<vector<double> > ng(X, vector<double>(Y, 0));
	for (int i = 0; i < iteration; i++) {
		for (size_t y = 0; y < Y; y++) {
			for (size_t x = 0; x < X; x++) {
				if (g[x][y] == 0)
					continue;
				ng[x][y] = (g[x-1][y]>0?(g[x-1][y] / NODE_L(x-1, y)):0) +
						   (g[x+1][y]>0?(g[x+1][y] / NODE_L(x+1, y)):0) +
						   (g[x][y-1]>0?(g[x][y-1] / NODE_L(x, y-1)):0) +
						   (g[x][y+1]>0?(g[x][y+1] / NODE_L(x, y+1)):0);
			}
		}
		/**
		 * applying new values
		 */
		total = 0;
		for (size_t y = 0; y < Y; y++)
			for (size_t x = 0; x < X; x++)
			{
				g[x][y] = ng[x][y];
				ng[x][y] = 0;
				total += g[x][y];
			}
		g[src.first][src.second] = total;
	}
	for (size_t y = 0; y < Y; y++)
		for (size_t x = 0; x < X; x++) {
			g[x][y] = g[x][y]/total;
		}
}

bool reachableNeighbor(coord_t box_p, coord_t&r, vector<vector<sq_t> >&b) {
	r = box_p;
	if (IS(b[++(r.first)][(r.second)], reachable))
		return true;
	r = box_p;
	if (IS(b[--(r.first)][(r.second)], reachable))
		return true;
	r = box_p;
	if (IS(b[(r.first)][++(r.second)], reachable))
		return true;
	r = box_p;
	if (IS(b[(r.first)][--(r.second)], reachable))
		return true;
	return false;
}
char interpret(sq_t s, bool flag) {
	if (IS(s,wall)) {
		return '#';
	}
	if (!flag) {
		if (IS(s,reachable)) {
			return '-';
		}
	}
	if (IS(s,sokoban)) {
		if (s & S_T(goal)) {
			return '+';
		}
		return '@';
	}
	if (IS(s,box)) {
		if (IS(s,reachable)) {
			return 'M';
		}
		if (IS(s,goal)) {
			return '*';
		}
		return '$';
	}
	if (flag) {
		if (IS(s,h_ddlock)) {
			if (IS(s,goal)) {
				return 'O';
			}
			return 'X';
		}
		if (IS(s,s_ddlock)) {
			if (IS(s,goal)) {
				return 'o';
			}
			return 'x';
		}
	}
	if (IS(s,goal)) {
		return '.';
	}
	if (IS(s,reachable)) {
		return '-';
	}
	return ' ';

}
vector<coord_t> flood_fill(vector<vector<sq_t> >&B, coord_t&s, int am, ...) {
	vector<sq_t> blk;
	va_list vl;
	va_start(vl,am);
	for (int i = 0; i < am; i++) {
		const sq_t bb = va_arg(vl, int);
		blk.push_back(bb);
	}
	va_end(vl);
	return flood_fill(B, s, blk);
}
vector<coord_t> flood_fill(vector<vector<sq_t> >&B, coord_t&s, vector<sq_t>&blk) {
	pos_t Y(B[0].size());
	pos_t X(B.size());
	for (pos_t y = 1; y < Y - 1; y++) {
		for (pos_t x = 1; x < X - 1; x++) {
			REM(B[x][y],reachable);
		}
	}
	vector<coord_t> movables;

	stack<coord_t> q;
	q.push(s);
#define EXPANDABLE(X_,Y_) (!( IS(B[X_][Y_],wall) || IS(B[X_][Y_],reachable) ))
	while (!q.empty()) {
		const pos_t x = q.top().first;
		const pos_t y = q.top().second;
		q.pop();
		if (!blk.empty()) {
			bool break_loop = false;
			for (vector<sq_t>::iterator it = blk.begin(); it != blk.end(); it++) {
				if (B[x][y] & *it) {
					break_loop = true;
					break;
				}
			}
			if (break_loop)
				break;
		}
		ADD(B[x][y],reachable);
		if (IS(B[x][y],box)) {
			movables.push_back(coord_t(x, y));
		} else {
			if (y < Y - 1 && EXPANDABLE(x, y + 1)) {
				q.push(coord_t(x, y + 1));
			}
			if (y > 0 && EXPANDABLE(x, y - 1)) {
				q.push(coord_t(x, y - 1));
			}
			if (x < X - 1 && EXPANDABLE(x + 1, y )) {
				q.push(coord_t(x + 1, y));
			}
			if (x > 0 && EXPANDABLE(x - 1, y)) {
				q.push(coord_t(x - 1, y));
			}
		}
	}
	return movables;
}
ostream& operator<<(ostream&os, const vector<vector<sq_t> >&sb) {
	for (pos_t y = 0; y < sb[0].size(); y++) {
		for (pos_t x = 0; x < sb.size(); x++) {
			os << interpret(sb[x][y], false);
		}
		os << endl;
	}
	return os;
}
ostream& operator<<(ostream&os, const StaticBoard&sb) {
	for (pos_t y = 0; y < sb.Y(); y++) {
		for (pos_t x = 0; x < sb.X(); x++) {
			os << interpret(sb.board()[x][y]);
		}
		os << endl;
	}
	return os;
}

ostream& operator<<(ostream&os, const coord_t&s) {
	return os << "(" << int(s.first) << "," << int(s.second) << ")";
}
ostream& operator<<(ostream&os, const direction&d) {
	switch (d) {
	case RIGHT:
		os << "R";
		break;
	case LEFT:
		os << "L";
		break;
	case UP:
		os << "U";
		break;
	case DOWN:
		os << "D";
		break;
	default:
		os << "~";
	}
	return os;
}
