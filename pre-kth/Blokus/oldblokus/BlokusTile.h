#ifndef BLOKUSTILE_H
#define BLOKUSTILE_H

#include "Tile.h"

class BlokusTile: public Tile
{
	public:
		inline BlokusTile(u8 width, u8 height):Tile(width, height) {}

		inline u8 getAmount() { return Amount; }
		inline void setAmount(u8 amount) { Amount = amount; }

		inline u8 getScore() { return Score; }

	protected:
		u8 Amount;
		u8 Score; //Amount of team blocks
};

#endif

