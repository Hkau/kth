#ifndef FIELD_H
#define FIELD_H

#include "Tile.h"
#include <cstdio>
class Field: public Tile
{
	public:
		inline Field(u8 width, u8 height):Tile(width, height){}
};

#endif

