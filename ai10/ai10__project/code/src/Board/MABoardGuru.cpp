/*
 * MABoardGuru.cpp
 *
 *  Created on: Oct 17, 2010
 *      Author: soheil
 */

#include "MABoardGuru.h"
#include "MABoardGraph.h"
MABoardGuru::MABoardGuru(StaticBoard*sb_, bool f, double w):TimeStopSolutionGuru(w), forward(f), sb(sb_), gp(f?sb_->goal_pos():sb_->box_pos()) {
}

bool MABoardGuru::matches(AStar *n){
	MABoardGraph&ma_b = *reinterpret_cast<MABoardGraph*> (n);
	vector<coord_t> aa = ma_b.b->box_pos();
	const vector<vector<pos_t> >& ii= ma_b.b->forward?sb->goal_i_index():sb->box_i_index();
	for(vector<coord_t>::iterator it=aa.begin();it!=aa.end();it++){
		if(ii[it->first][it->second] == 255)
			return false;
	}
	if(!forward){
		coord_t des_sok = ma_b.b->get_sok();
		return (IS(sb->board()[des_sok.first][des_sok.second], reachable));
	}
	return true;
}
double MABoardGuru::heuristic(AStar *n) {
	return 0;
}

MABoardGuru::~MABoardGuru() {
}
