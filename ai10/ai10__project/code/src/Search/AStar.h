/*
 * AStar.h
 *
 *  Created on: Oct 15, 2010
 *      Author: soheil
 */

#ifndef ASTARNODE_H_
#define ASTARNODE_H_
#include <ctime>
#include <list>
#include <map>
#include <algorithm>
#include <iostream>
#include <boost/shared_ptr.hpp>

class AStar;

typedef boost::shared_ptr<AStar> AStarPtr;
class SolutionGuru {
public:
	virtual bool matches(AStar *n)=0;
	virtual double heuristic(AStar *n) {
		return 0;
	}
	virtual bool giveup(AStar *n) {
		return false;
	}
};

class AStar {
public:
	AStar();
	static bool find_path(SolutionGuru&s_guru, AStarPtr&start, AStarPtr&goal);
	AStarPtr Parent() {
		return parent;
	}
	virtual ~AStar();
	virtual bool equals(AStarPtr&other)=0;
protected:
	virtual size_t hash()=0;
	virtual void expand_children(std::list<std::pair<AStarPtr, double> > &l) = 0;
	virtual void close();
	AStarPtr parent;
	double costG;
	double costH;
private:
	/*
	 * Adds an element to open list, sort guaranteed
	 */
	static void addToOpenList(std::multimap<size_t, AStarPtr> &open, std::multimap<double, AStarPtr>& open_sorted, AStarPtr&n);
	static void addPathElement(AStarPtr&n, double cost, AStarPtr&parent,
			std::multimap<size_t, AStarPtr> &open,
			std::multimap<double, AStarPtr>& open_sorted,
			std::multimap<size_t, AStarPtr> &closed, SolutionGuru&s_guru);
	static AStarPtr moveFromOpenToClose(std::multimap<size_t, AStarPtr> &open,
			std::multimap<double, AStarPtr>& open_sorted,
			std::multimap<size_t, AStarPtr> &closed);
};

class TimeStopSolutionGuru: public SolutionGuru {
private:
	time_t t1;
	time_t t2;
	const double wait;
public:
	TimeStopSolutionGuru(double w) :
		wait(w) {
		time(&t1);
	}
	virtual bool matches(AStar *n)=0;
	virtual double heuristic(AStar *n) {
		return 0;
	}
	virtual bool giveup(AStar *n) {
		time(&t2);
		return wait > 0 ? difftime(t2, t1) > wait : false;
	}
};
class IterStopSolutionGuru: public SolutionGuru {
private:
	const size_t max_iter;
	size_t cur_iter;
public:
	IterStopSolutionGuru(size_t mi) :
		max_iter(mi) {
		cur_iter = 0;
	}
	virtual bool matches(AStar *n)=0;
	virtual double heuristic(AStar *n) {
		return 0;
	}
	virtual bool giveup(AStar *n) {
		return max_iter > 0 ? ++cur_iter > max_iter : false;
	}
};

#endif /* ASTARNODE_H_ */
