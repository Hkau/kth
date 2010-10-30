/*
 * BoardLocal.h
 *
 *  Created on: Oct 16, 2010
 *      Author: soheil
 */

#ifndef BOARDLOCAL_H_
#define BOARDLOCAL_H_
#include <vector>
#include <list>

#include "Board.h"

list<direction> searchPath(coord_t sok_s, coord_t box_s,coord_t sok_e, coord_t box_e, vector<vector<sq_t> >&con, size_t max_iter=0);
list<direction> searchPath(coord_t sok_s, coord_t box_s,coord_t box_e, vector<vector<sq_t> >&con, size_t max_iter=0);
#endif /* BOARDLOCAL_H_ */
