/*
 * Board.cpp
 *
 *  Created on: Oct 16, 2010
 *      Author: soheil
 */

#include "Board.h"

Board::Board(StaticBoard*p, bool f) :
	X(p->X()), Y(p->Y()), forward(f), _board(p->board()), sb(p), m_pos(
			forward ? sb->box_pos() : sb->goal_pos()), sok(sb->sok_pos()) {
	if (!forward) {
		for (pos_t y = 1; y < sb->Y() - 1; y++) {
			for (pos_t x = 1; x < sb->X() - 1; x++) {
				if (!IS(_board[x][y], wall))
					ADD(_board[x][y], reachable);
				if (IS(_board[x][y], goal) && !IS(_board[x][y], box)) {
					REM(_board[x][y], goal);
					ADD(_board[x][y], box);
				} else if (!IS(_board[x][y], goal) && IS(_board[x][y], box)) {
					ADD(_board[x][y], goal);
					REM(_board[x][y], box);
				}
			}
		}
		sok = make_pair(-1, -1);
	}
}
Board::Board(const Board&ob) :
	X(ob.X), Y(ob.Y), forward(ob.forward), _board(ob._board), sb(ob.sb), m_pos(
			ob.m_pos), sok(ob.sok) {
}

bool Board::make_action(action_t a) {
	if (!is_valid_action(a))
		return false;

	REM(_board[m_pos[a.first].first][m_pos[a.first].second], box);
	if (sok.first < X)
		REM(_board[sok.first][sok.second], sokoban);

	if (forward) {
		sok = m_pos[a.first];
		m_pos[a.first] = to_be_free;
	} else {
		sok = to_be_free;
		m_pos[a.first] = sok_des;
	}

	ADD(_board[sok.first][sok.second], sokoban);
	ADD(_board[m_pos[a.first].first][m_pos[a.first].second], box);
	flood_fill(_board, sok, 0);
	return true;
}

vector<action_t> Board::pos_action() {
	vector<action_t> p;
	action_t the_action;
	for (pos_t b_id = 0; b_id < m_pos.size(); b_id++) {
		if (!IS(_board[m_pos[b_id].first][m_pos[b_id].second], reachable))
			continue;
		the_action = make_pair(b_id, RIGHT);
		if (is_valid_action(the_action)) {
			p.push_back(the_action);
		}
		the_action = make_pair(b_id, LEFT);
		if (is_valid_action(the_action)) {
			p.push_back(the_action);
		}
		the_action = make_pair(b_id, DOWN);
		if (is_valid_action(the_action)) {
			p.push_back(the_action);
		}
		the_action = make_pair(b_id, UP);
		if (is_valid_action(the_action)) {
			p.push_back(the_action);
		}
	}
	return p;
}
/*
 * Assumed that reachability to each node is already calculated
 */
bool Board::is_valid_action(action_t a) {
	const bool log = false;
	if (log)
		cerr << "Check if " << "(" << m_pos[a.first] << (forward ? "->" : "-<")
				<< a.second << ") is valid? ";

	if (!IS(_board[m_pos[a.first].first][m_pos[a.first].second], box)) {
		if (log)
			cerr << "No!" << endl;
		return false;
	}
	sok_des = m_pos[a.first];
	to_be_free = m_pos[a.first];

	switch (a.second) {
	case RIGHT:
		if (forward) {
			sok_des.first--;
			to_be_free.first++;
		} else {
			sok_des.first++;
			if(to_be_free.first>=X-2) return false;
			to_be_free.first += 2;
		}
		break;
	case LEFT:
		if (forward) {
			sok_des.first++;
			to_be_free.first--;
		} else {
			if(to_be_free.first<2) return false;
			sok_des.first--;
			to_be_free.first -= 2;
		}
		break;
	case DOWN:
		if (forward) {
			if(sok_des.second==0) return false;
			sok_des.second--;
			to_be_free.second++;
		} else {
			if(to_be_free.second>=Y-2) return false;
			sok_des.second++;
			to_be_free.second += 2;
		}
		break;
	case UP:
		if (forward) {
			sok_des.second++;
			to_be_free.second--;
		} else {
			if(to_be_free.second<2) return false;
			sok_des.second--;
			to_be_free.second -= 2;
		}
		break;
	default:
		if (log)
			cerr << "No!" << endl;
		return false;
	}

	if (forward) {
		if (IS(_board[to_be_free.first][to_be_free.second], h_ddlock)
				&& !IS(_board[to_be_free.first][to_be_free.second], goal)) {
			if (log)
				cerr << "No!" << endl;
			return false;
		}
	}else{
		if (!IS(_board[to_be_free.first][to_be_free.second], reachable)) {
			if (log)
				cerr << "No!" << endl;
			return false;
		}
	}

	if (!(IS(_board[sok_des.first][sok_des.second], reachable)
			&& !IS(_board[sok_des.first][sok_des.second], box))) {
		if (log)
			cerr << "No!" << endl;
		return false;
	}

	if (IS(_board[to_be_free.first][to_be_free.second], wall)
			||IS(_board[to_be_free.first][to_be_free.second], box)) {
		if (log)
			cerr << "No!" << endl;
		return false;
	}

	if (log)
		cerr << "Yes!" << endl;
	return true;
}
void Board::freeze(coord_t pos) {
	ADD(_board[pos.first][pos.second], wall);
}
void Board::unfreeze(coord_t pos) {
	REM(_board[pos.first][pos.second], wall);
}

size_t Board::hash() const {
	size_t _hash = 5381;
	pos_t x, y;
	for (y = 1; y < Y - 1; y++)
		for (x = 1; x < X - 1; x++) {

			size_t elem_id;
			if (IS(_board[x][y], box))
				elem_id = 7 * x;
			else if (IS(_board[x][y], reachable))
				elem_id = 33 * y;
			else
				elem_id = x * y;

			_hash = ((_hash << 5) + _hash) + elem_id;
		}
	return _hash;
}

bool Board::operator ==(const Board&ob) const {
	for (pos_t x = 0; x < X; x++)
		for (pos_t y = 0; y < Y; y++) {
			if (IS(_board[x][y], box))
				if (!IS(ob._board[x][y], box))
					return false;
			if (IS(_board[x][y], reachable) && !IS(ob._board[x][y], reachable))
				return false;
		}
	return true;
}
Board::~Board() {
	// TODO Auto-generated destructor stub
}
ostream& operator<<(ostream& os, const Board&b) {
	for (pos_t y = 0; y < b.Y; y++) {
		for (pos_t x = 0; x < b.X; x++)
			os << interpret(b._board[x][y]);
		os << endl;
	}
	return os;
}
ostream& operator<<(ostream& os, const action_t&a) {
	return os << int(a.first) << a.second;
}
