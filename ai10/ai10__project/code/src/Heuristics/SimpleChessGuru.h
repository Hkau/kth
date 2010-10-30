/*
 * SimpleChessGuru.h
 *
 *  Created on: Oct 17, 2010
 *      Author: soheil
 */

#ifndef SIMPLECHESSGURU_H_
#define SIMPLECHESSGURU_H_

#include "../Board/MABoardGuru.h"

#define DIFF(X,Y) ( (X)>(Y)?((X)-(Y)):((Y)-(X)))
#define DIST(X,Y) ( DIFF((X).first,(Y).first)+DIFF((X).second,(Y).second) )

class SimpleChessGuru: public MABoardGuru {
public:
	SimpleChessGuru(StaticBoard*sb_, bool f, double w=61):MABoardGuru(sb_, f, w){}
	virtual double heuristic(AStar *n){
		MABoardGraph&ma_b = *reinterpret_cast<MABoardGraph*> (n);
		vector<coord_t> box=ma_b.b->box_pos();

		double estimated_cost = 0;
		for(vector<coord_t>::iterator it=gp.begin();it!=gp.end();it++){
			size_t mind = ~0;
			size_t mini = ~0;
			for(size_t i=0;i<box.size();i++){
				if( DIST(*it,box[i]) < mind){
					mini = i;
					mind = DIST(*it,box[i]);
				}
			}
			box.erase(box.begin()+mini);
			estimated_cost+=mind;
		}
		return estimated_cost;
	}
};

#endif /* SIMPLECHESSGURU_H_ */
