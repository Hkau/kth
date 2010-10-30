/*
 * MABoardGuru.h
 *
 *  Created on: Oct 17, 2010
 *      Author: soheil
 */

#ifndef MABOARDGURU_H_
#define MABOARDGURU_H_

#include "../Search/AStar.h"
#include "StaticBoard.h"

class MABoardGuru: public TimeStopSolutionGuru {
public:
	MABoardGuru(StaticBoard*sb_, bool f, double w=61);
	virtual bool matches(AStar *n);
	virtual double heuristic(AStar *n);
	virtual ~MABoardGuru();
	StaticBoard*get_static_board(){return sb;}
	const bool forward;
protected:
	StaticBoard*sb;
	vector<coord_t> gp;
	vector<coord_t> bp;
};

#endif /* MABOARDGURU_H_ */
