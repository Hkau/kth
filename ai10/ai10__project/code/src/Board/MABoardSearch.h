/*
 * MABoardSearch.h
 *
 *  Created on: Oct 17, 2010
 *      Author: soheil
 */

#ifndef MABOARDSEARCH_H_
#define MABOARDSEARCH_H_

#include <list>
#include "StaticBoard.h"
#include "MABoardGraph.h"
#include "MABoardGuru.h"

#define FORWARD true
#define BACKWARD false

list<action_t> MultiAgentBoardSearch(MABoardGuru&guru);
list<direction> Multi2SingleAgent(list<action_t> ma_a, StaticBoard*s);
ostream&operator<<(ostream&os, list<direction>&d);
#endif /* MABOARDSEARCH_H_ */
