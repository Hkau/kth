/*
 * ReachabilityGuru.h
 *
 *  Created on: Oct 17, 2010
 *      Author: soheil
 */

#ifndef REACHABILITYGURU_H_
#define REACHABILITYGURU_H_
#include <set>
#include "../Board/MABoardGuru.h"

class ReachabilityGuru: public MABoardGuru {
private:
	vector<vector<vector<size_t> > > reach;
	vector<vector<size_t> > gbr;
	vector<set<pos_t> > gbr_set;
public:
	ReachabilityGuru(StaticBoard*sb_, bool f, double w = 61) :
		MABoardGuru(sb_, f, w) {
		sb->calculate_goal_reachability();
		sb->calculate_goal_box_reachability();

		gbr = sb->goal_box_reachability();
		if (f) {
			reach = vector<vector<vector<size_t> > > (sb->goal_reachability());
		} else {
			sb->calculate_goal_i_reachability();
			reach
					= vector<vector<vector<size_t> > > (
							sb->goal_i_reachability());
		}
		gbr_set = vector<set<pos_t> > (gp.size(), set<pos_t> ());
		for (pos_t it = 0; it < gp.size(); it++) {
			for (pos_t i = 0; i < gp.size(); i++) {
				if (gbr[it][i] != 0) {
					if (forward)
						gbr_set[it].insert(i);
					else
						gbr_set[i].insert(it);
				}
			}
		}
		set<size_t> done;
		size_t s=~0;

		while (s != done.size()) {
			s = done.size();
			for (pos_t it = 0; it < gp.size(); it++) {
				if (done.count(it) != 0)
					continue;
				if (gbr_set[it].size() == 1) {
					done.insert(it);
					for (pos_t i = 0; i < gp.size(); i++) {
						if (done.count(i) != 0)
							continue;
						gbr_set[i].erase(it);
					}
				}
			}
		}
	}
	virtual double heuristic(AStar *n) {
		MABoardGraph&ma_b = *reinterpret_cast<MABoardGraph*> (n);
		vector<coord_t> box = ma_b.b->box_pos();

		double estimated_cost = 0;

		for (size_t it = 0; it < gp.size(); it++) {
			size_t mind = ~0;

			for (set<pos_t>::iterator sit = gbr_set[it].begin(); sit
					!= gbr_set[it].begin(); sit++) {

				size_t distance;

				if (forward) {
					distance = reach[box[*sit].first][box[*sit].second][it];
				} else {
					distance = reach[box[*sit].first][box[*sit].second][it];
				}

				if (distance < mind) {
					mind = distance;
				}
			}
			estimated_cost += mind;
		}
		return estimated_cost;
	}
};

#endif /* SIMPLECHESSGURU_H_ */
