/*
 * MABoardGraph.h
 *
 *  Created on: Oct 17, 2010
 *      Author: soheil
 */

#ifndef MABOARDGRAPH_H_
#define MABOARDGRAPH_H_

#include "../Search/AStar.h"
#include "Board.h"

class MABoardGraph: public AStar {
private:
	size_t _hash;
public:
	MABoardGraph(const Board&board, action_t action);
	virtual void expand_children(std::list<std::pair<AStarPtr, double> > &l);
	virtual bool equals(AStarPtr&other);
	virtual void close();
	virtual size_t hash();
	virtual ~MABoardGraph();
	const action_t last_action;
	Board*b;
};

#endif /* MABOARDGRAPH_H_ */
