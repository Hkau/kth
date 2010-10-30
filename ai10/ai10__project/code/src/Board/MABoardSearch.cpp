/*
 * MABoardSearch.cpp
 *
 *  Created on: Oct 17, 2010
 *      Author: soheil
 */

#include "MABoardSearch.h"
list<direction> findSokPath(coord_t sok_s, coord_t sok_e, vector<vector<sq_t> >&con);
void make_reverse_action(action_t&act, map<pos_t, pos_t>&m);

list<action_t> MultiAgentBoardSearch(MABoardGuru&guru) {
	StaticBoard*sb = guru.get_static_board();
	bool forward = guru.forward;

	Board start_board(sb, forward);
	AStarPtr goal, start = AStarPtr(new MABoardGraph(start_board, NO_ACTION));
	if (AStar::find_path(guru, start, goal)) {
		cout << "Solution found!" << endl;
		MABoardGraph* the_node = reinterpret_cast<MABoardGraph*> (goal.get());
		map<pos_t, pos_t> goal_box_map;
		if (!forward) {
			vector<coord_t> bp = the_node->b->box_pos();
			for (pos_t bi = 0; bi < bp.size(); bi++) {
				goal_box_map[bi]
						= sb->box_i_index()[bp[bi].first][bp[bi].second];
			}
		}
		list<action_t> res;
		while (the_node != NULL) {
			action_t act = the_node->last_action;
			if (!forward) {
				make_reverse_action(act, goal_box_map);
				res.push_back(act);
			}else{
				res.push_front(act);
			}
			the_node
					= reinterpret_cast<MABoardGraph*> (the_node->Parent().get());
		}
		return res;
	} else {
		if(guru.giveup(NULL))
			cout << "Giving up! Maximum time exceeded" << endl;
		else
			cout << "No solution found!" << endl;
		return list<action_t> ();
	}
}

list<direction> Multi2SingleAgent(list<action_t> ma_a, StaticBoard*s) {
	Board start_board(s, FORWARD);
	coord_t sok;
	list<direction> solution;
	for(list<action_t>::iterator ma_it=ma_a.begin();ma_it!=ma_a.end();ma_it++)
	{
		if(ma_it->second == NONE)
			continue;
		sok = start_board.sok;
		vector<vector<sq_t> > bb(start_board._board);
		start_board.make_action(*ma_it);
		list<direction> ac = findSokPath(sok,start_board.sok_des,bb);
		cout << "Action: " << *ma_it << "->" << ac << ma_it->second << endl;
		solution.insert(solution.end(), ac.begin(), ac.end());
		solution.push_back(ma_it->second);
	}
	return solution;
}

void make_reverse_action(action_t&act, map<pos_t, pos_t>&m) {
	act.first = m[act.first];
	switch (act.second) {
	case RIGHT:
		act.second = LEFT;
		break;
	case LEFT:
		act.second = RIGHT;
		break;
	case DOWN:
		act.second = UP;
		break;
	case UP:
		act.second = DOWN;
		break;
	default:
		break;
	}
}

class SokMover: public AStar {
private:
	size_t _hash;
public:
	coord_t sok;
	direction last_dir;
	vector<vector<sq_t> >*brd;
	SokMover(coord_t s, vector<vector<sq_t> >*brr, direction ld = NONE) :
			_hash(5381), sok(s), last_dir(ld), brd(brr) {
		_hash = ((_hash << 5) + _hash) + s.first;
		_hash = ((_hash << 5) + _hash) + s.second;
	}
	virtual size_t hash() {
		return _hash;
	}
	virtual void expand_children(std::list<std::pair<AStarPtr, double> > &l) {
		/*
		 * Move to right
		 */
		{
		coord_t nxt = sok;
		nxt.first++;
		if(!IS((*brd)[nxt.first][nxt.second], wall)&&!IS((*brd)[nxt.first][nxt.second], box))
				l.push_back(make_pair(AStarPtr(new SokMover(nxt, brd,RIGHT)), 1));
		}
		/*
		 * Move to left
		 */
		{
		coord_t nxt = sok;
		nxt.first--;
		if(!IS((*brd)[nxt.first][nxt.second], wall)&&!IS((*brd)[nxt.first][nxt.second], box))
				l.push_back(make_pair(AStarPtr(new SokMover(nxt, brd,LEFT)), 1));
		}
		/*
		 * Move to up
		 */
		{
		coord_t nxt = sok;
		nxt.second--;
		if(!IS((*brd)[nxt.first][nxt.second], wall)&&!IS((*brd)[nxt.first][nxt.second], box))
				l.push_back(make_pair(AStarPtr(new SokMover(nxt, brd,UP)), 1));
		}
		/*
		 * Move to down
		 */
		{
		coord_t nxt = sok;
		nxt.second++;
		if(!IS((*brd)[nxt.first][nxt.second], wall)&&!IS((*brd)[nxt.first][nxt.second], box))
				l.push_back(make_pair(AStarPtr(new SokMover(nxt, brd,DOWN)), 1));
		}

	}
	virtual bool equals(AStarPtr&other) {
		SokMover*n = reinterpret_cast<SokMover*> (other.get());
		return (sok == n->sok);
	}
};
#define DIFF(X,Y) ( (X)>(Y)?((X)-(Y)):((Y)-(X)) )
#define DIST(X,Y) ( DIFF((X).first,(Y).first)+DIFF((X).second,(Y).second) )
class SokMoverGuru: public SolutionGuru {
private:
	coord_t sok;
public:
	SokMoverGuru(coord_t s) :
		sok(s) {
	}
	virtual bool matches(AStar *nn) {
		SokMover*n = reinterpret_cast<SokMover*> (nn);
		return (n->sok == sok);
	}
	virtual double heuristic(AStar *nn) {
		SokMover*n = reinterpret_cast<SokMover*> (nn);
		return DIST(n->sok, sok);
	}
};
list<direction> findSokPath(coord_t sok_s, coord_t sok_e, vector<vector<sq_t> >&con) {

	SokMoverGuru sol(sok_e);
	AStarPtr goal, start = AStarPtr(new SokMover(sok_s, &con));
	if (AStar::find_path(sol, start, goal)) {
		SokMover* the_node = reinterpret_cast<SokMover*> (goal.get());

		list<direction> res;
		while (the_node != NULL) {
			res.push_front(the_node->last_dir);
			the_node = reinterpret_cast<SokMover*> (the_node->Parent().get());
		}
		res.pop_front();
		return res;
	} else {
		return list<direction> ();
	}
}

ostream&operator<<(ostream&os, list<direction>&d){
	for (list<direction>::iterator it = d.begin(); it != d.end(); it++) {
		os << *it << " ";
	}
	return os;
}
