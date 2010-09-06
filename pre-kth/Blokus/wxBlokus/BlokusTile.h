#ifndef BLOKUSTILE_H
#define BLOKUSTILE_H

#include "Tile.h"

//! Blokus tile
/** Wraps tile data and adds amount and score to it. */
class BlokusTile
{
	public:
		//! Get amount
		/** Returns amount of tiles. */
		inline u8 getAmount() { return Amount; }

		//! Set amount
		/** Sets amount of tiles. */
		inline void setAmount(u8 amount) { Amount = amount; }

		//! Get score
		/** Gets score per tiles. */
		inline u8 getScore() { return Score; }

		//! Set score
		/** Sets score per tiles. */
		inline void setScore(u8 score) { Score = score; }

		//! Get Tile data
		/** Returns pointer to actual tile data. */
		inline Tile *getTileData() { return TileData; }

		//! Set Tile data
		/** Associates BlokusTile with actual data. */
		inline void setTile(Tile *tile) { TileData = tile; }

	protected:
		Tile *TileData; // Actual tile data
		u8 Amount; // Amount the player has of the block
		u8 Score; // Score the block gives
};

#endif

