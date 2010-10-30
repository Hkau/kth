/*
 * BoardLocal.cpp
 *
 *  Created on: Oct 16, 2010
 *      Author: soheil
 */

#include <math.h>
#include <map>
#include "../Search/AStar.h"

#include "BoardLocal.h"
#define DIST_EUQ(X,Y) sqrt( ((X).first-(Y).first)*((X).first-(Y).first) + ((X).second-(Y).second)*((X).second-(Y).second) )
#define DIFF(X,Y) ( (X)>(Y)?((X)-(Y)):((Y)-(X)) )
#define DIST(X,Y) ( DIFF((X).first,(Y).first)+DIFF((X).second,(Y).second) )

/*
 * AStar node and Guru classes are defined here inside
 * the source code (not in header) as they are only used
 * here and is better not to be visible anywhere else.
 */
class SimpleSok: public AStar {
private:
	size_t _hash;
public:
	coord_t sok, box;
	direction last_dir;
	vector<vector<sq_t> >*brd;
	SimpleSok(coord_t s, coord_t b, vector<vector<sq_t> >*brr, direction ld =
			NONE) : _hash(5381),
		sok(s), box(b), last_dir(ld), brd(brr) {
		_hash = ((_hash << 5) + _hash) + s.first;
		_hash = ((_hash << 5) + _hash) + s.second;
		_hash = ((_hash << 5) + _hash) + b.first;
		_hash = ((_hash << 5) + _hash) + b.second;
	}
	virtual size_t hash(){
			return _hash;
	}
	virtual void expand_children(std::list<std::pair<AStarPtr, double> > &l) {
		/*
		 * Move to right
		 */
		{
			coord_t nxt = sok;
			coord_t nbx = box;
			;
			nxt.first++;
			if (!IS((*brd)[nxt.first][nxt.second], wall)) {
				if (nxt == box) {

					nbx.first++;
					if (!IS((*brd)[nbx.first][nbx.second], wall)) {
						l.push_back(make_pair(AStarPtr(new SimpleSok(nxt, nbx,
								brd, RIGHT)), 1));
					}
				} else {
					l.push_back(make_pair(AStarPtr(new SimpleSok(nxt, box, brd,
							RIGHT)), 1));
				}
			}
		}
		/*
		 * Move to left
		 */
		{
			coord_t nxt = sok;
			coord_t nbx = box;
			;
			nxt.first--;
			if (!IS((*brd)[nxt.first][nxt.second], wall)) {
				if (nxt == box) {
					nbx.first--;
					if (!IS((*brd)[nbx.first][nbx.second], wall)) {
						l.push_back(make_pair(AStarPtr(new SimpleSok(nxt, nbx,
								brd, LEFT)), 1));
					}
				} else {
					l.push_back(make_pair(AStarPtr(new SimpleSok(nxt, box, brd,
							LEFT)), 1));
				}
			}
		}
		/*
		 * Move to up
		 */
		{
			coord_t nxt = sok;
			coord_t nbx = box;
			;
			nxt.second--;
			if (!IS((*brd)[nxt.first][nxt.second], wall)) {
				if (nxt == box) {
					nbx.second--;
					if (!IS((*brd)[nbx.first][nbx.second], wall)) {
						l.push_back(make_pair(AStarPtr(new SimpleSok(nxt, nbx,
								brd, UP)), 1));
					}
				} else {
					l.push_back(make_pair(AStarPtr(new SimpleSok(nxt, box, brd,
							UP)), 1));
				}
			}
		}
		/*
		 * Move to down
		 */
		{
			coord_t nxt = sok;
			coord_t nbx = box;
			nxt.second++;
			if (!IS((*brd)[nxt.first][nxt.second], wall)) {
				if (nxt == box) {
					nbx.second++;
					if (!IS((*brd)[nbx.first][nbx.second], wall)) {
						l.push_back(make_pair(AStarPtr(new SimpleSok(nxt, nbx,
								brd, DOWN)), 1));
					}
				} else {
					l.push_back(make_pair(AStarPtr(new SimpleSok(nxt, box, brd,
							DOWN)), 1));
				}
			}
		}
	}
	virtual bool equals(AStarPtr&other) {
		SimpleSok*n = reinterpret_cast<SimpleSok*> (other.get());
		return (sok == n->sok) && (box == n->box);
	}
};

class SokSolGuru: public IterStopSolutionGuru {
private:
	coord_t sok, box;
public:
	SokSolGuru(coord_t s, coord_t b, size_t m_count) :
		IterStopSolutionGuru(m_count), sok(s), box(b) {
	}
	virtual bool matches(AStar *nn) {
		SimpleSok*n = reinterpret_cast<SimpleSok*> (nn);
		return (n->box == box) && (n->sok == sok);
	}
	virtual double heuristic(AStar *nn) {
		SimpleSok*n = reinterpret_cast<SimpleSok*> (nn);
		return DIST(n->box,box) + DIST(n->sok,sok);
	}
};
class BoxMoverSolGuru: public IterStopSolutionGuru {
private:
	coord_t box;
public:
	BoxMoverSolGuru(coord_t b, size_t m_count) :
		IterStopSolutionGuru(m_count), box(b) {
	}
	virtual bool matches(AStar *nn) {
		SimpleSok*n = reinterpret_cast<SimpleSok*> (nn);
		return (n->box == box);
	}
	virtual double heuristic(AStar *nn) {
		SimpleSok*n = reinterpret_cast<SimpleSok*> (nn);
		return DIST(n->box,box);
	}
};

list<direction> searchPath(coord_t sok_s, coord_t box_s, coord_t sok_e,
		coord_t box_e, vector<vector<sq_t> >&con, size_t max_iter) {

	SokSolGuru sol(sok_e, box_e, max_iter);
	AStarPtr goal, start = AStarPtr(new SimpleSok(sok_s, box_s, &con));

	if (AStar::find_path(sol, start, goal)) {
		SimpleSok* the_node = reinterpret_cast<SimpleSok*> (goal.get());

		list<direction> res;
		while (the_node != NULL) {
			res.push_front(the_node->last_dir);
			the_node = reinterpret_cast<SimpleSok*> (the_node->Parent().get());
		}
		res.pop_front();
		return res;
	} else {
		return list<direction> ();
	}
}
list<direction> searchPath(coord_t sok_s, coord_t box_s, coord_t box_e, vector<
		vector<sq_t> >&con, size_t max_iter) {

	BoxMoverSolGuru sol(box_e, max_iter);
	AStarPtr goal, start = AStarPtr(new SimpleSok(sok_s, box_s, &con));
	//cerr << "Path from " << box_s << " to " << box_e << " starting from " << sok_s << " ";
	if (AStar::find_path(sol, start, goal)) {
		//cerr << "found!" ;
		SimpleSok* the_node = reinterpret_cast<SimpleSok*> (goal.get());

		list<direction> res;
		while (the_node != NULL) {
			res.push_front(the_node->last_dir);
			the_node = reinterpret_cast<SimpleSok*> (the_node->Parent().get());
		}

		return res;
	} else {
		//cerr << "not found!" << endl;
		return list<direction> ();
	}
}
