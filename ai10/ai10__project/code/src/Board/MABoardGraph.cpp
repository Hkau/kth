/*
 * MABoardGraph.cpp
 *
 *  Created on: Oct 17, 2010
 *      Author: soheil
 */

#include "MABoardGraph.h"
#include <cstdio>
MABoardGraph::MABoardGraph(const Board&board, action_t action):last_action(action) {
	b = new Board(board);
	b->make_action(action);
	_hash = b->hash();
}
size_t MABoardGraph::hash() {
	return _hash;
}
void MABoardGraph::expand_children(std::list<std::pair<AStarPtr, double> > &l)
{
	vector<action_t> pos_actions = b->pos_action();
	for(vector<action_t>::iterator act_it=pos_actions.begin();act_it!=pos_actions.end(); act_it++)
	{
		MABoardGraph* c = new MABoardGraph(*b, *act_it);
		coord_t new_box = c->b->box_pos()[act_it->first];
		if(c->b->goal_i_index()[new_box.first][new_box.second]!=255){
			c->b->freeze(new_box); // freeze its box and ad another copy
			l.push_back(make_pair(AStarPtr( new MABoardGraph(*b, *act_it) ), 1));
		}
		l.push_back(make_pair(AStarPtr( c ), 1));
	}
}
bool MABoardGraph::equals(AStarPtr&other)
{
	return _hash== reinterpret_cast<MABoardGraph*>(other.get())->hash();
}
void MABoardGraph::close()
{
	if(b!= NULL)
		delete b;
	b = NULL;
}

MABoardGraph::~MABoardGraph() {
}
