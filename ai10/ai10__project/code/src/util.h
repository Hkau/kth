/*
 * util.h
 *
 *  Created on: Oct 16, 2010
 *      Author: soheil
 */

#ifndef UTIL_H_
#define UTIL_H_
#include <vector>
#include <set>
#include <iostream>
using namespace std;
template<typename T>
ostream& operator<<(ostream&os, const vector<vector<T> >&g) {
	size_t X(g.size());
	size_t Y(g[0].size());
	for (size_t y = 0; y < Y; y++) {
		for (size_t x = 0; x < X; x++) {
			os << g[x][y] << " ";
		}
		os << endl;
	}
	return os;
}
template<typename T>
ostream& print3DMatrix(ostream&os, const vector< vector<vector<T> > >& ms, int slice) {
	size_t X(ms.size());
	size_t Y(ms[0].size());
	size_t Z(ms[0][0].size());
	if (slice<Z)
	for (size_t y=0;y<Y;y++) {
		for (size_t x=0;x<X;x++) {
			cerr << ms[x][y][slice] << " ";
		}
		os << endl;
	}
	return os;
}
#endif /* UTIL_H_ */
