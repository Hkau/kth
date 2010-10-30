/*
 * AStar.cpp
 *
 *  Created on: Oct 15, 2010
 *      Author: soheil
 */

#include <utility>
#include "AStar.h"
#include <math.h>

inline bool operator==(AStarPtr & a, AStarPtr & b) {
	return a->equals(b);
}

AStar::AStar() {
}

bool AStar::find_path(SolutionGuru&s_guru, AStarPtr&start, AStarPtr&goal) {

	std::multimap<double, AStarPtr> open_sorted;
	std::multimap<size_t, AStarPtr> open;

	std::multimap<double, AStarPtr>::iterator top_open;

	std::multimap<size_t, AStarPtr> closed;

	addToOpenList(open, open_sorted, start);

	while (!open.empty()) {

		AStarPtr node = moveFromOpenToClose(open, open_sorted, closed);

		if (s_guru.matches(node.get())) {
			goal = node;
			return true;
		} else {
			if (s_guru.giveup(node.get())) {
				break;
			}
			std::list<std::pair<AStarPtr, double> > conns;
			node->expand_children(conns);
			for (std::list<std::pair<AStarPtr, double> >::iterator e =
					conns.begin(); e != conns.end(); e++) {
				double costF = node->costG + e->second;
				addPathElement(e->first, costF, node, open, open_sorted,
						closed, s_guru);
			}
			node->close();
		}
	}
	return false;
}
void AStar::addToOpenList(std::multimap<size_t, AStarPtr> &open, std::multimap<
		double, AStarPtr>& open_sorted, AStarPtr&n) {

	open_sorted.insert(std::make_pair(n->costG + n->costH, n));
	open.insert(std::make_pair(n->hash(), n));
}

void AStar::addPathElement(AStarPtr&n, double cost, AStarPtr&parent,
		std::multimap<size_t, AStarPtr> &open,
		std::multimap<double, AStarPtr>& open_sorted, std::multimap<size_t,
				AStarPtr> &closed, SolutionGuru&s_guru) {

	std::pair<std::multimap<size_t, AStarPtr>::iterator, std::multimap<size_t,
			AStarPtr>::iterator> ret;
	std::multimap<size_t, AStarPtr>::iterator it;

	ret = open.equal_range(n->hash());
	if (ret.first != open.end()) {
		for (it = ret.first; it != ret.second; it++) {
			if (n->equals(it->second)) {
				if (it->second->costG > cost) {
					open.erase(it);
					break;
				} else {
					return;
				}
			}
		}
	}

	ret = closed.equal_range(n->hash());
	if (ret.first != closed.end()) {
		for (it = ret.first; it != ret.second; it++) {
			if (n->equals(it->second)) {
				if (it->second->costG > cost) {
					closed.erase(it);
					break;
				} else {
					return;
				}
			}
		}
	}

	// Create new node or use found one from list
	n->parent = parent; // Parent node
	n->costG = cost; // cost from start
	n->costH = s_guru.heuristic(n.get()); // Guessed cost to target

	addToOpenList(open, open_sorted, n);
}
AStarPtr AStar::moveFromOpenToClose(std::multimap<size_t, AStarPtr> &open,
		std::multimap<double, AStarPtr>& open_sorted, std::multimap<size_t,
				AStarPtr> &closed) {

	std::pair<std::multimap<size_t, AStarPtr>::iterator, std::multimap<size_t,
			AStarPtr>::iterator> ret;

	std::multimap<size_t, AStarPtr>::iterator it;

	AStarPtr n = open_sorted.begin()->second;

	ret = open.equal_range(n->hash());
	for (it = ret.first; it != ret.second; it++) {
		if (n->equals(it->second)) {
			open.erase(it);
			break;
		}
	}

	open_sorted.erase(open_sorted.begin());
	closed.insert(std::make_pair(n->hash(), n));
	return n;
}

void AStar::close() {
}

AStar::~AStar() {
	close();
}
